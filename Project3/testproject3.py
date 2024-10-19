from importmngr import *
from project3 import Model


def main():
    """
    This function will run the code requirements asked for in the project three description. \n
    All functions are written in the ProjectThreeDriver file, this file only contains calls to Model Class in that file
    """

    # Q1)   Read train.csv
    filepath = rf"C:\Users\WN84801\Downloads\train.csv"

    TrainingModel1 = Model(filepath)
    TrainingModel2 = Model(filepath)
    TrainingModel3 = Model(filepath)
    # Must initialize three models as to allow for all later questioned be answered

    # Q2)  Basic Data Analytics
    print("Question 2\n")

    print(f"{TrainingModel1.sizeDF()}\n"
          f"{TrainingModel1.lengthDF()}\n"
          f"{TrainingModel1.meanPrice()}\n"
          f"{TrainingModel1.minPrice()} \n"
          f"{TrainingModel1.maxPrice()} \n"
          f"{TrainingModel1.stdPrice()} \n")

    # Q3) Price Histogram

    TrainingModel1.histogramPrice()

    # Q4) Feature Pairwise scatter

    TrainingModel1.PairPlotDF()

    # Q5-9 are in Project3Driver

    # Q10)

    z = TrainingModel1.TrainModel(0.2)
    iterations_mod1 = range(len(z))
    plot_1 = plt.plot(iterations_mod1, z, label='Learning Rate = 0.2', color='green')
    plt.legend()
    plt.show()
    plt.close()

    # Q 11/12)
    a = TrainingModel2.TrainModel(10 ** -7)
    b = TrainingModel3.TrainModel(10 ** -7.5)

    iterations_mod2 = range(len(a))
    iterations_mod3 = range(len(b))

    print(f" Final Mean Square Error for model a {TrainingModel2.loss().sum()}\n"
          f"Final Mean Square Error for model b {TrainingModel3.loss().sum()}")

    plt.plot(iterations_mod2, a, label='Learning Rate = 10**-7', color='red')
    plt.plot(iterations_mod3, b, label='Learning Rate = 10**6.5', color='blue')
    plt.legend()
    plt.show()
    plt.close()

    # Q13)
    if TrainingModel2.loss().sum() < TrainingModel3.loss().sum():
        print("Question 13: Swapping Data Sets")
        TrainingModel2.changeDataSet(newData=r"C:\Users\WN84801\Downloads\test.csv")
        select_model = TrainingModel2
        predictions = TrainingModel2.pred()
        Ids = TrainingModel2.DF[['Id']]
        final_predictions = pd.merge(Ids, predictions.to_frame(), left_index=True, right_index=True)
    else:
        print("Question 13: Swapping Data Sets")
        TrainingModel3.changeDataSet(newData=r"C:\Users\WN84801\Downloads\test.csv")
        select_model = TrainingModel3
        predictions = TrainingModel3.pred()
        Ids = TrainingModel3.DF[["Id"]]
        final_predictions = pd.merge(Ids, predictions.to_frame(), left_index=True, right_index=True)
        final_predictions.columns = ['Ids', 'Predicted Price']
    print("MSE on the testing dataset %s" % select_model.loss().sum())

    # Save final predictions to Excel file
    with pd.ExcelWriter('Final_Predictions.xlsx') as writer:
        final_predictions.to_excel(writer, index=False)


if __name__ == "__main__":
    main()
