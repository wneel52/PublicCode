/*****************************************************************
William Neel
ECE 231 - Professor David McLaughlin

Lab 8.2 - OLED Display and UART Display
This script will take the temperature from the TMP36 sensor and display it on the OLED display and UART terminal. 
The temperature will be displayed in both Celsius and Fahrenheit. The script will also toggle an LED if the temperature exceeds 80 degrees Fahrenheit.
*******************************************************************/

// Include libraries
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "SSD1306.h"
#include "my_adc_lib.h"
#include "my_uart_lib.h"

// Define pins
#define button PD7 // pin 3
#define REDLED PD5 // pin 5

// Function prototypes
void displayC(double C, char buffer[]);

void displayF(double F, char buffer[]);

void sendC(double C, char buffer[]);

void sendF(double F, char buffer[]);

void toggleHot(float highTemp, double F);

// main function
int main(void){
    
    // Initialize variables
    char buffer[6];
    uart_init();
    OLED_Init(); 
    adc_init();
    float highTemp = 80.00;

    // Set up the LED and button
    DDRD = 1<<REDLED; // set pin 5 to output
    PORTD = 1<<button; // enable internal pull resistor 
    unsigned int digValue;
    
    // Main loop
    while (1){
        // Read the button state
        unsigned char state = (PIND & (1<<button));
        digValue = get_adc();   
        double mv = (double)((digValue/1024.0)*1.1);
        double C = (mv - 0.5)*100;
        double F = (C*(9.0/5))+32;
    
        // Toggle the LED if the temperature exceeds 80 degrees Fahrenheit    
        toggleHot(highTemp, F);

        // Display the temperature in Celsius or Fahrenheit
        if(state){
            // Display the temperature in Fahrenheit
            displayF(F, buffer);
            sendF(F, buffer);
        }
        else{
            // Display the temperature in Celsius
            displayC(C, buffer);
            sendC(C, buffer);
        }
        // Delay for 1 second
        _delay_ms(1000);

    }
}

// Convert a float to a string using a character buffer
// buffer: the buffer to store the string
// F: the float to convert
void floatToString(char* buffer, double F){

    int integerPart = (int)F;
    double decimalPart = F - (double)integerPart;
    int decimalPartInt = (int)(decimalPart * 10); // 2 decimal places
    sprintf(buffer, "%d.%d", integerPart, decimalPartInt);

}

// Send the temperature in Celsius to the UART terminal
// C: the temperature in Celsius
// buffer: the buffer to store the string
void sendC(double C, char buffer[]){
    
    floatToString(buffer, C);
    int len = strlen(buffer);
    buffer[len] = 'C'; //appends a C to the end of the string
    buffer[len+1] = '\0'; //null terminator
    send_string("Temp in C: ");
    send_string(buffer);
    uart_send(13); //send carriage return
    uart_send(10); //end line feed

}

// Send the temperature in Fahrenheit to the UART terminal
// F: the temperature in Fahrenheit
// buffer: the buffer to store the string
void sendF(double F, char buffer[]){

    floatToString(buffer, F);
    int len = strlen(buffer);
    buffer[len] = 'F'; //appends a C to the end of the string
    buffer[len+1] = '\0'; //null terminator
    send_string("Temp in F: ");
    send_string(buffer);
    uart_send(10); //end line feed
    uart_send(13); //send carriage return

}

// Toggle the LED if the temperature exceeds 80 degrees Fahrenheit
// highTemp: the temperature threshold
// F: the temperature in Fahrenheit
void toggleHot(float highTemp, double F){
    if (F > highTemp) {
        PORTD |= (1<<REDLED); // Turn on the LED
    } 
    else {
        PORTD &= ~(1<<REDLED); // Turn off the LED
    }
}

// Display the temperature in Celsius to the OLED display
// buffer: the buffer to store the string
// C: the temperature in Celsius
void displayC(double C, char buffer[]){
    floatToString(buffer, C);
    int len = strlen(buffer);
    buffer[len] = 'C'; //appends a C to the end of the string
    buffer[len+1] = '\0'; //null terminator
    OLED_GoToLine(4);
    OLED_DisplayString("Temp in C: ");
    OLED_GoToLine(6);
    OLED_DisplayString(buffer);
}

// Display the temperature in Fahrenheit to the OLED display
// buffer: the buffer to store the string
// F: the temperature in Fahrenheit
void displayF(double F, char buffer[]){ 
    floatToString(buffer, F);
    int len = strlen(buffer);
    buffer[len] = 'F'; //appends a C to the end of the string
    buffer[len+1] = '\0'; //null terminator
    OLED_GoToLine(4);
    OLED_DisplayString("Temp in F: ");
    OLED_GoToLine(6);
    OLED_DisplayString(buffer);
}

/*** End of File***/