from importmngr import *


class Model:

    def __init__(self, filepath: str):
        """
         Linear Regression Model, takes in a .CSV filepath as a str, this is to create a DataFrame Object
        :param filepath: .CSV Filepath as a string *** Must be a raw string ***
        """

        # Create Dataframe Object
        self.filepath = filepath
        self.DF = self.loadDF()

        # Create Feature Vector
        self.feature_vector = self.DF.drop(['Price', 'Id'], axis=1)

        # Randomly initialize weights
        self.weight = np.random.rand(1, 25)

    def loadDF(self):
        """
        :return: Creates DataFrame object from .CSV filepath
        """
        return pd.read_csv(self.filepath)

    def changeDataSet(self, newData=None):
        if newData is None:
            newData = input("Enter a new .csv filepath: ")
        self.filepath = rf"{newData}"
        self.DF = self.loadDF()

    def headDF(self):
        """
        Prints the first 10 rows of the dataframe: known as the "head" of the dataframe
        """
        return self.DF.head(10)

    def lengthDF(self):
        """
        Calls the pandas size function on the Id column to count the entries
        :return: Number of entries in the DF
        """
        return " The data frame has {0} entire".format(self.DF["Id"].size)

    def sizeFV(self):
        """
        Calls pandas size function on the feature vector
        :return: the size of the feature vector
        """
        return "The feature vector has {0} features".format(self.feature_vector.size)

    def sizeDF(self):
        """
        Calls Pandas "size" function on the price dataset
        :return: the number of entries in a given dataframe
        """
        return "The dataframe has {0} entries".format(self.DF.size)

    def meanPrice(self):
        """
        Calls Pandas "mean()" function on the price dataset
        :return: the mean of the price column
        """
        return "Mean of all price entries {0}".format(self.DF['Price'].mean())

    def maxPrice(self):
        """
        Calls Pandas "max()" function on the price dataset
        :return: the maximum price of the data set
        """
        return "The maximum price of the entire dataset {0}".format(self.DF['Price'].max())

    def minPrice(self):
        """
        Calls Pandas "min()" function on the price dataset
        :return: the minimum price of the data set
        """
        return "The minimum price of the entire dataset {0}".format(self.DF['Price'].min())

    def stdPrice(self):
        """
        Calls Pandas "std()" function on the price dataset
        :return: Standard Deviation of the price dataset
        """
        return "The standard deviation of the price dataset {0}".format(self.DF['Price'].std())

    def histogramPrice(self):
        """
        Creates a histogram of the houses price with the darkness of a block being related to how expensive it is
        the darker the hue the more expensive a house
        :return: None
        """
        warnings.filterwarnings("ignore", category=UserWarning, module="seaborn")
        warnings.filterwarnings("ignore", category=FutureWarning, module="seaborn")
        plt.figure(figsize=(10, 6))
        seaborn.histplot(data=self.DF, x='Price', bins=30, hue='Price', legend=False)

        plt.xlabel('House Price')
        plt.ylabel('Frequency')
        plt.title('Distribution of Housing Prices in the Dataset')
        plt.show()
        plt.close()

    def PairPlotDF(self):
        """
        Creates a Pair-Wise Plot of the Feature Vector to enable us to see if there is any correlation between
        the feature vectors
        :return: None
        """
        warnings.filterwarnings("ignore", category=FutureWarning, module="seaborn")
        warnings.filterwarnings("ignore", category=UserWarning, module="seaborn")

        seaborn.pairplot(
            self.DF,
            vars=["GrLivArea", "BedroomAbvGr", "TotalBsmtSF", "FullBath"],
            kind="scatter",
            diag_kind="kde",
            hue='Price')

        plt.show()
        plt.close()

    def pred(self):
        """
        Multiplies the features value by the weight value, then adds them into one value representing the prediction
        :return: Prediction of the house price
        """
        prediction = self.feature_vector.multiply(self.weight)

        return prediction.sum(axis=1)

    def loss(self):
        """
        The loss function of the Linear Regression Model, here we are using Mean Square Error (MSE) to calculate
        the loss. We subtract the original price from the prediction, square the value and divide by the array length
        :return: MSE of each data point as a [1xLengthData] Dataframe
        """
        prediction = self.pred()
        loss = (prediction - self.DF['Price']).pow(2).multiply(1 / self.DF['Price'].size)

        return loss

    def gradient(self):
        """
        The gradient function of the Linear Regression Model, this is the derivative of our loss function, which we
        will then use to calculate the updated values of the weight. Here we calculate the prediction - price(y_y)
        again we then transpose our feature vector and multiply it by y_y then by 2/ArrayLength
        :return: The gradient of the iteration as a [1 x Length of Feature Vector] array
        """
        prediction = self.pred()
        y_y = (prediction - self.DF['Price'])

        feature_vector_transposed = self.feature_vector.T

        gradient = feature_vector_transposed.multiply(y_y).multiply(2 / self.DF['Price'].size).sum(axis=1)

        return gradient.T.to_list()

    def update(self, alpha):
        """
        The Updated weight equals the original weight minus the original weight times alpha
        :param alpha: The learning rate of the algorithm, will be passed in by the "TrainModel" Function
        :return: Updated Weight Vector
        """
        gradient = self.gradient()

        for i, j in zip(range(len(self.weight)), gradient):
            self.weight[i] = self.weight[i] - (alpha * j)

        return self.weight

    def TrainModel(self, alpha, num_iterations=500):
        """
        Trains the Linear Regression Model, will call update function to update weights for the number of given
        iterations, we use a for loop to handel this. If the loss drops below the given threshold (0.2) the algorithm
        will stop before the given number of iterations
        :param alpha: Learning Rate of the Algorithm, will determine convergence
        :param num_iterations: Number of iterations the Algorithm
        :return: Mean Square Error of each iteration
        """
        mse = []
        iter_count = 0

        for _ in range(num_iterations):
            iter_count += 1
            self.update(alpha)
            current_loss = self.loss().sum()  # Calculate the current total loss
            mse.append(current_loss)
            # Add a stopping condition (you can customize this based on your needs)
            if current_loss < 0.2:
                print("Stopping training. Loss is below threshold.")
                break

        print(f"Training completed after {iter_count} iterations.")
        formatted_weights = [float('{:.4f}'.format(float(w))) for w in self.weight.flatten()]

        print("Final weights:", formatted_weights)

        return mse
