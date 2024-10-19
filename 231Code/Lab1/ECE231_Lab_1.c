/*
ECE 231 Lab 1 - This script will take in 10 user input integers into an array
the script will then create a timestamp from the CPU and calculate the mean of the input array
following that we will output the time and mean

Date                Author           Revision
02/12/2024        William Neel    initial version
*/
#include <stdio.h>
#include <time.h>

struct timeNow{
        time_t tv_sec; //    seconds
        long tv_nsec; // nanoseconds
};


void process(int Arr[], int ArrSize,  int *mean,  struct timespec *timeNow );

int main(){
    
    struct timespec timeNow;
    int pass; // Controlls TimeStamp 
    int temp; 
    int in;
    int buffer[10];
    int uB = 9000;
    int lB = 0;
    int mean;
    
    for (int round = 0; round < 2; round++){
        pass = 0;

        for (int i = 0; i < 10; i++){

            printf("Enter int #%d :\n",(i+1));
            if (scanf("%d",&in)==0){
                printf("Error Reading input: Only enter intgers \n");
                printf("\n");
                printf("end process \n");
                break; }

            else{                
            if (in < lB || in > uB){
                if (in < lB){
                    printf("Invalid Number \n Please enter an int greater than 0 \n %d < 0 \n", in); 
                    printf("1");
                    continue;}
                else{
                    printf("Invalid Number \n Please enter an int less than 9000 \n %d > 9000 \n", in);
                    printf("2"); 
                    continue; } }
            
            buffer[i] = in;
                
            for (int j = 0; j < i; j++){
                if (buffer[i]==buffer[j]){
                    printf("Value %d = %d No overlap allowed \n Enter a new value: \n",buffer[i],buffer[j]);
                    i--; j--;
                    continue; } } 
            /*else 41*/} /* For line 29*/}

               
        for (int q = 0; q < 10; q++){
            if (buffer[q]<0 || buffer[q]>9000){
                printf("Error: Invalid value in array. PLease only inter intgers: \n");
                printf("Cannot compute mean with invalid entries: Run script again \n");
                break; }
            else {pass++;}    
        if (pass==10){
            process(buffer,10,&mean,&timeNow);
            printf("Iteration %d: Mean: %d TimeStamp: %lu\n",round+1,mean,(timeNow.tv_sec+(timeNow.tv_nsec/10^9)));
            } }
        
        } 
        return 0;
        }

void process(int Arr[], int ArrSize, int *mean, struct timespec *timeNow){   
       
    int sum = 0;
    for (int i = 0; i < 10; i++){
        sum = sum + Arr[i];
    }              
    
    *mean = (double)sum/ArrSize;     
    clock_gettime(CLOCK_MONOTONIC, timeNow); 
}

