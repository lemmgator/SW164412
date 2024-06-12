#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "buttons.h"
#include "adc.h"
#define FCY             4000000UL

void __delay_us(unsigned long us){
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms){
    __delay32(FCY / 1000 * ms);
}
unsigned int adc, power, start = 0;
volatile unsigned int time, min, sec, default_time = 300;

int main(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);

    char buffer[32];
    LCD_Initialize(); 
    
    time = default_time;
    
    while(time > 0) {
        adc = ADC_ReadPercentage(ADC_CHANNEL_POTENTIOMETER);
        power = adc * 10;
        min = time / 60;
        sec = time % 60;
        if(BUTTON_IsPressed(BUTTON_S6) == true){
            time = time + 30;
        }
        if(BUTTON_IsPressed(BUTTON_S3) == true){
            time = time - 30;
            if(time > 1800){
                break;
            }
        }
        if(BUTTON_IsPressed(BUTTON_S5) == true){
            if(start == 0){
                start = 1;
            }
            else{
                start = 0;
            }
        } 
        if(BUTTON_IsPressed(BUTTON_S4) == true){
            start = 0;
            time = default_time;
        }
        sprintf(buffer, "%02u:%02u           %04uW", min, sec, power);
        
        LCD_ClearScreen();
        LCD_PutString(buffer, strlen(buffer));

        if(time > 1800){
            time = 1800;
        }
        if(start == 1){
            time--;
            __delay_ms(500);
        }
    }
    
    sprintf(buffer, "00:00     KONIEC%04uW", power);
    LCD_ClearScreen();
    LCD_PutString(buffer, strlen(buffer));
    __delay_ms(2500); 
    return 0;
}