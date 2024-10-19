#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "SSD1306.h"
#include "my_adc_lib.h"
#include "my_uart_lib.h"
#include <math.h>

// Define pins
#define button PD7 // pin 3
#define REDLED PD5 // pin 5

// Function prototypes
void f2s(char* strArray, double num);
// main function

double adcToC(unsigned int adc){
    double mv = (double)((adc/1024.0)*1.1);
    double C = (mv-0.5)*100;
    return C;
}

double adcToF(unsigned int adc){
    double C = adcToC(adc);
    double F = (C*(9.0/5))+32;
    return F;
}


int main(void){
    
    // Initialize variables
    char stringArray[10];
    uart_init();
    OLED_Init(); 
    adc_init();
    float highTemp = 26.666667;

    DDRD = 1<<REDLED; // set pin 5 to output
    PORTD = 1<<button; // enable internal pull resistor 
    unsigned int digValue;
    
    // Main loop
    while (1){
        // Read the button state
        unsigned char buttonPress = (PIND & (1<<button));
        digValue = get_adc();   
        double C = adcToC(digValue);
        double F = adcToF(digValue);
    
        if (C < highTemp){
            PORTD &= ~(1<<REDLED); // Turn off the LED
        } 
        else {
            PORTD |= (1<<REDLED); // Turn on the LED
        }

        // Display the temperature in Celsius or Fahrenheit
        if(!buttonPress){
            
            // Display the temperature in Fahrenheit
            f2s(stringArray, F);
            strcat(stringArray, " F");
            
            OLED_GoToLine(4);
            OLED_DisplayString("Temp in F: ");
            OLED_GoToLine(6);
            OLED_DisplayString(stringArray);

            send_string("Temp in F: ");
            send_string(stringArray);
            uart_send(13); //send carriage return
            uart_send(10); //end line feed 
            
            
        }
        else{
            // Display the temperature in Celsius
            f2s(stringArray, C);
            strcat(stringArray, " C");
            
            OLED_GoToLine(4);
            OLED_DisplayString("Temp in C: ");
            OLED_GoToLine(6);
            OLED_DisplayString(stringArray);

            send_string("Temp in C: ");
            send_string(stringArray);
            uart_send(13); //send carriage return
            uart_send(10); //end line feed 
        }
        _delay_ms(1000);

    }
}

void f2s(char* strArray, double num){

    int intComp = (int)num;
    double floatComp = num - (double)intComp;
    int displayDec = (int)(round(floatComp * 100)); // 2 decimal places
    sprintf(strArray, "%d.%d", intComp, displayDec);

}
