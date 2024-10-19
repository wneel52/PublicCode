/**
 ECE 231 Lab #2: Blinking LED
 

 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

bool createfile(FILE **timeDiff);

int main() {
    FILE *timeDiff;
    FILE *LEDHandle = NULL;
    const char *LEDpath = "/sys/class/leds/beaglebone:green:usr3/brightness";
    int i = 0;
    long a, b;
    struct timespec timeNow;
    char currentState;

    // If file is not created, exit the program
    if (!createfile(&timeDiff)) {
        printf("File not unable to be created: \n");
        return 1;
    }

    printf("Begin Program \n");
    while (i < 1000) {
        printf("iteration %d\n", (i + 1));

        // Create first timestamp
        clock_gettime(CLOCK_MONOTONIC, &timeNow);
        // Assign to 'a'
        a = timeNow.tv_nsec;
        usleep(500000);

        // Open LED file in read-write mode
        if ((LEDHandle = fopen(LEDpath, "r+")) != NULL) {
            // Read a single character from the file
            int toggle = fscanf(LEDHandle, "%c", &currentState);
            if (toggle == 1) {
                printf("LED state: %c\n", currentState);
                if (currentState == '1') {
                    // Turn the LED off
                    fwrite("0", sizeof(char), 1, LEDHandle); 
                    } 
                else {
                    // Turn the LED on
                    fwrite("1", sizeof(char), 1, LEDHandle); 
                    }
            }
            // Close the LED file
            fclose(LEDHandle);
        }

        // Take second timestamp
        clock_gettime(CLOCK_MONOTONIC, &timeNow);
        // Assign to 'b'
        b = timeNow.tv_nsec;
        // Write absolute difference to timeDiff file
        fprintf(timeDiff, "%ld\n", abs(b - a));

        i++;
    }

    // Close timeDiff after while loop
    fclose(timeDiff);
    printf("End Process: \n");
    return 0;
}

bool createfile(FILE **timeDiff) {
    // Create or override "time_diff_file.txt" in write mode
    *timeDiff = fopen("time_diff_file.txt", "w");

    // Check if file can be opened or created
    if (*timeDiff == NULL) {
        printf("Error opening file: \n");
        return false;
    }

    // True if file is opened or created
    return true;
}