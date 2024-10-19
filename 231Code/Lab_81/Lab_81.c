/* 
William Neel 

4/14/24

Professor David McLaughlin

Lab Assignment 8.1
This script will use an Arduino Uno
We will be handling three buttons; two directly connected to LEDs and one to operate both LEDs at once 
When pressing the two connected direcetly the LEDs will turn off.


*/


// Import Declarations
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// Variable Declarations
#define REDBUTTON PD3   // Arduino Uno pin 3
#define GREENBUTTON PD4 // Arduino Uno pin 4
#define REDLED PD6      // Arduino Uno pin 6
#define GREENLED PD7    // Arduino Uno pin 7
#define SB PD5 // Arduino pin 5
#define eepy 200

// Function Declarations
void singleState();
void dualState();
void offState();


int main(void){

    DDRD = 1<<REDLED|1<<GREENLED;           
    PORTD = 1<<REDBUTTON|1<<GREENBUTTON|1<<SB;
               
    while(1){

        unsigned char r_button, g_button, s_button;
        r_button = (PIND & (1<<REDBUTTON));     // 1 when not pressed
        g_button = (PIND & (1<<GREENBUTTON));   // 0 when pressed
        s_button = (PIND & (1<<SB)); 

        if(!r_button && !g_button){
            offState();    // if two buttons are pressed
        }
        else if(!s_button){
            dualState();
        }
        else{
            singleState();
        }
        
    }
    return 0;
}

/* When two buttons are pressed at the same time we enter the "Off State"
sets both LED output pins to low*/
void offState(){

    PORTD &= ~(1<<REDLED);       // Turn off red LED
    PORTD &= ~(1<<GREENLED);
}

/*
Dual state flashes both LEDs simultaneously from one button
Can be interrupted by another button press
*/
void dualState(){      
    PORTD &= ~(1<<REDLED);       // Turn off red LED
    PORTD &= ~(1<<GREENLED);
    _delay_ms(eepy);    // delay 
    PORTD |= (1<<REDLED);   // turn on LED                       
    PORTD |= (1<<GREENLED);
    _delay_ms(eepy);    // delay 
}

/*
Single State is when the LEDs operate seperate from eachother
LEDs will operate at 5Hz 
If both are pressed LEDs will enter "Off State"
*/
void singleState(){

    DDRD = 1<<REDLED|1<<GREENLED;           
    PORTD = 1<<REDBUTTON|1<<GREENBUTTON|1<<SB;
    unsigned char r_button, g_button, s_button;
    r_button = (PIND & (1<<REDBUTTON));     // 1 when not pressed
    g_button = (PIND & (1<<GREENBUTTON));   // 0 when pressed
    s_button = (PIND & (1<<SB));
    
    if  (r_button){   
        PORTD &= ~(1<<REDLED);      // Off when button not pressed
    }
    else{
        PORTD |= (1<<REDLED); // On when pressed
        _delay_ms(eepy);    // delay
        PORTD &= ~(1<<REDLED);  // off
        _delay_ms(eepy);    //delay            
    }
    if  (g_button){                 
        PORTD &= ~(1<<GREENLED);     // Off when not pressed
    } 
    else {
        PORTD |= (1<<GREENLED);       // On when pressed  
        _delay_ms(eepy);    // delay
        PORTD &= ~(1<<GREENLED);    // off 
        _delay_ms(eepy);    // delay           
    }
}