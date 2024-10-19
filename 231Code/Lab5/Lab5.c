/*
William Neel
ECE 231: Lab #5
This script will use a multithreaded approach and interrupts to record 
time and tempature readings.
*/

// Basic header files
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Thread and Events libs
#include <pthread.h> 
#include <sys/epoll.h> // only vaild on BBB -> throws error on windows machine
// importing method to read sensor values from another c file
#include "tempFunc.c"

// Global Varibales
struct sensor_type{
    struct timespec timestamp;
    double celcius_temp;
};

struct sensor_type buffer[10];

pthread_mutex_t lock;

//Function Declaration

/* 
Creates txt file, returns true if process completes, false otherwise
Allows Users to control errors with file creation with boolen operators
*/
bool createfile(FILE **fp);

/*
Input Thread: Event Polling for a rising edge interrupt
If interrupt is recived system will take a time stamp,
then we will get a  reading from the temp sensor 
*/
void *inputThread(void* Array){
    
    struct sensor_type* buffer = (struct sensor_type*)Array;
    
    int gpio_number = 67;
    printf("Begin Input Thread \n");
    // declare char to be path to interrupt file
    char InterruptPath[40];
    // assign Interrupt path
    sprintf(InterruptPath, "/sys/class/gpio/gpio%d/value", gpio_number);
    // looking to detect a change to the state of the value file.

    // epoll varibales
    int epfd;
    struct epoll_event ev;

    // abstraction to manipulate files
    FILE *fp = fopen(InterruptPath, "r");
    // unix linux file ID used for system calls
    int fd = fileno(fp);
    // this is the event we will monitor
    
    // monitor the output of the file 
    epfd = epoll_create(1);

    // Register Events to be monitored
    // events we are monitoring are input to the file and an edge triggered event 
    ev.events = EPOLLIN | EPOLLET;
    
    // register interrupt file with epoll to monitor 
    ev.data.fd = fd;

    epoll_ctl(epfd,EPOLL_CTL_ADD, fd, &ev);

    int capture_interrupt;
    struct epoll_event ev_wait;

    pthread_mutex_lock(&lock);
    
    for(int i=0; i < 10; i++){
        capture_interrupt = epoll_wait(epfd, &ev_wait, 1, -1);
        clock_gettime(CLOCK_MONOTONIC, &buffer[i].timestamp);
        buffer[i].celcius_temp = rtTemp(); 
    }

    pthread_mutex_unlock(&lock);
    
    printf("End Input Thread Process \n");
    
    fclose(fp);
    
    close(epfd);
    
}            

/*
Output thread:
When input array is filled output thread will aquire the buffer
then print buffer info to the terminal and append to a txt file
*/
void *outputThread(void* Array){
    // current implemtation is overwriting the previous input: a global varibale that tracks up to 50 could help.
    // embed the for loop in a while (X < 50); then each time the for loop  
        
    FILE *sensor_data = fopen("William_Neel_sensordata.txt","a"); // open file in "append mode" -> need to test if implementation is effective
    // should just skip to last open line and "append" to the end of file.
    struct sensor_type* buffer = (struct sensor_type*)Array;
    
    pthread_mutex_lock(&lock);

    // will need nanosecond percision when calculating the timestamp -> total of 5 seconds and we need distinct data points thus nsecond percision.
    for(int i = 0; i < 10; i++){

        uint64_t timestamp_ns = ((uint64_t)buffer[i].timestamp.tv_sec * 1000000000) + (uint64_t)buffer[i].timestamp.tv_nsec);

        printf("Timestamp: %llu | Temp in C: %f\n", timestamp_ns ,buffer[i].celcius_temp);
        fprintf(sensor_data, "%llu|%f\n", timestamp_ns, buffer[i].celcius_temp);
  
    }
    pthread_mutex_unlock(&lock);

}

bool createfile(FILE **fp){
    // Create or override "time_diff_file.txt" in write mode
    *fp = fopen("William_Neel_sensordata.txt", "a");   

    // Check if file can be opened or created
    if (*fp == NULL) {
        printf("Error opening file: \n");
        return false;
    }
    // True if file is opened or created
    return true;
}

/*
Main Function:
Controlls thread logic / cycling between input and output thread
*/
int main(){

    int getData = 50;
    
    FILE *sensor_data;
    // If file is not created, exit the program
    if (!createfile(&sensor_data)) {
        printf("File not unable to be created: \n");
        exit(-1);
    }

    pthread_t input_thread, output_thread;

    // initialize mutex lock to be used in threads
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    // while loop controlled by getData int, if successfully compliled will lower getData by 10 -> which is the size of the array being processed each time
    while (getData > 0){
        
        if (pthread_create(&output_thread, NULL, outputThread, (void*)(&buffer)) !=0 ){
            printf("Error Creating output thread: end process \n");
            return -1;
        }    
        
        if (pthread_create(&input_thread, NULL, inputThread, (void*)(&buffer)) !=0 ){
            printf("Error Creating input thread: end process \n");
            return -1;
        }

        if (pthread_join(output_thread, NULL) !=0 ){
            printf("Error joining output thread: end process \n");
            return -1;
        }
        
        if (pthread_join(input_thread, NULL) !=0 ){
            printf("Error joining input thread: end process \n");
            return -1;
        }

        getData -= 10;
    }
    
    pthread_mutex_destroy(&lock);

    fclose(sensor_data);

    pthread_exit(0);

}