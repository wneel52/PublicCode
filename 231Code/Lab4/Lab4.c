/* 
William Neel: ECE231 Lab 4

This script will utilize event polling and interrupts to implement a thread based programming 
apprach to store the timestamps of the interrupts in an array then once full 

*/
// Basic Imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "configure_interrupt.c"

// Thread and Events libs
#include <pthread.h> 
#include <sys/epoll.h> // only vaild on BBB


void epollThread(void* Array){
    
    printf("Begin Thread Process \n");    
    long* Buffer = (long*)Array;

    int gpio_number = 67;
    char InterruptPath[40];
    sprintf(InterruptPath, "/sys/class/gpio/gpio%d/value", gpio_number);
    // looking to detect a change to the state of the value file.

    int epfd;
    struct epoll_event ev;

    // abstraction to manipulate files
    FILE *fp = fopen(InterruptPath, "r");
    // unix linux file ID used for system calls
    int fd = fileno(fp);
    // this is the event we will monitor
    
    epfd = epoll_create(1);

    // Register Events to be monitored
    ev.events = EPOLLIN | EPOLLET;
    
    // register interrupt file with epoll to monitor 
    ev.data.fd = fd;

    epoll_ctl(epfd,EPOLL_CTL_ADD, fd, &ev);

    int capture_interrupt;
    struct timespec tm;
    struct epoll_event ev_wait;

    for(int i=0; i < 10; i++){
        capture_interrupt = epoll_wait(epfd, &ev_wait, 1, -1);
        clock_gettime(CLOCK_MONOTONIC, &tm);
        Buffer[i] = tm.tv_sec;
        printf("Interrupt Recieved %d at %ld \n",capture_interrupt, tm.tv_sec);

    }
    close(epfd);
    printf("End Thread Process \n");
    
}

int main(){
    
    long Buffer[10];

    pthread_t thd_id;
    printf("Thread has NOT YET been executed \n");

    pthread_create(&thd_id, NULL, epollThread , void* (*Buffer));
    
    pthread_join(thd_id,NULL);
    
    printf("Thread Executed \n");
    
    for(int j = 0; j < 10; j++){
        printf("Entry %d, Value %ld \n",j+1,Buffer[j]);
    }
    
    pthread_exit(0);

}
