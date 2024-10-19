// main

// Include libraries
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "SSD1306.h"
#include "my_adc_lib.h"

// Define Constants
#define REDLED PD0;
#define YELLOWLED PD1;
#define GREENLED PD2;
#define BLUELED PD3;
#define SPEAKER PD4;

// Function Prototypes

void toggleLED(float tempValue, float actualTemp);
float getTempC(void);
float getTempF(float tempC);

// Main Function

int main(void){
    adc_init();

    while(1){

    double C = getTempC();
    double F = getTempF(C);
    toggleLED(F);

    }
    
    return 0;
}

// Function Definitions

// Function to toggle the LED based on the temperature
void toggleLED(float tempValue){
    
    float tolerence = 5.0;
    float lowTemp = 60.0;
    float highTemp = 80.0;

    if (tempValue < lowTemp){
        PORTD = 1<<BLUELED;
    }
    else if(tempValue > highTemp){
        PORTD = 1<<REDLED;
    }
    else if (tempValue > (highTemp-tolerence) || tempValue < (lowTemp+tolerence)){
        PORTD = 1<<YELLOWLED;
    }
    else{
        PORTD = 1<<GREENLED;
    }
    
}

// Function to get the temperature in Celsius
float getTempC(void){
    digitalValue = get_adc();
    double voltageValue = (double)((digitalValue/1023.0)*1.1);
    return ((voltageValue - 0.5) * 100);
}

// Function to get the temperature in Fahrenheit
float getTempF(float tempC){
    return (tempC * (9.0/5.0)) + 32;
}