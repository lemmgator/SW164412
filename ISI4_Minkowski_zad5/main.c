#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"        // Include LCD library
#include "buttons.h"    // Include buttons library
#define FCY             4000000UL

unsigned time_w, time_b, min_w, sec_w, min_b, sec_b, start = 0, total_min, next = 0, inc = 0, ff_w = 0, ff_b = 0, draw = 0;
volatile unsigned int total = 300;

void __delay_us(unsigned long us){
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms){
    __delay32(FCY / 1000 * ms);
}

int main(void) {
    char buffer[32];
    LCD_Initialize();
    
    while(start != 1){
        time_w = total;
        time_b = total;
        min_w = time_w / 60;
        sec_w = time_w % 60;
        min_b = time_b / 60;
        sec_b = time_b % 60;
        total_min = total / 60;
        sprintf(buffer, "%02u:%02u W  B %02u:%02u%01u|%01u", min_w, sec_w, min_b, sec_b, total_min, inc);
        LCD_ClearScreen();
        LCD_PutString(buffer, strlen(buffer));
        
        if(BUTTON_IsPressed(BUTTON_S3) == true){
            total -= 60;
            if (total == 0){
                total = 300;
            }
        }
        if(BUTTON_IsPressed(BUTTON_S6) == true){
            inc += 1;
            if (inc > 5){
                inc = 0;
            }
        }
        if(BUTTON_IsPressed(BUTTON_S5) == true){
            start = 1;
            break;
        }
    }
    while (time_w > 0 && time_b > 0) {        
        min_w = time_w / 60;
        sec_w = time_w % 60;
        min_b = time_b / 60;
        sec_b = time_b % 60;

        if (BUTTON_IsPressed(BUTTON_S3) == true) {
            next = 1;
            time_w += inc;
            if(time_w > total){
                time_w = total;
            }
        } else if (BUTTON_IsPressed(BUTTON_S4) == true) {
            next = 0;
            time_b += inc;
            if(time_b > total){
                time_b = total;
            }
        }
        if(next == 0){
            time_w--;
            sprintf(buffer, "%02u:%02u W< B %02u:%02u", min_w, sec_w, min_b, sec_b);
        }
        else{
            time_b--;
            sprintf(buffer, "%02u:%02u W >B %02u:%02u", min_w, sec_w, min_b, sec_b);
        }
        
        LCD_ClearScreen();
        LCD_PutString(buffer, strlen(buffer));
        
        if((BUTTON_IsPressed(BUTTON_S3) == true) && (BUTTON_IsPressed(BUTTON_S6) == true)){
            ff_w = 1;
            break;
        }
        if((BUTTON_IsPressed(BUTTON_S5) == true) && (BUTTON_IsPressed(BUTTON_S4) == true)){
            ff_b = 1;
            break;
        }
        if((BUTTON_IsPressed(BUTTON_S5) == true) && (BUTTON_IsPressed(BUTTON_S6) == true)){
            draw = 1;
            break;
        }
        __delay_ms(500);
    }
    if(time_b == 0 || ff_b == 1){
        sprintf(buffer, "%02u:%02u W  B 00:00WHITE WIN", min_w, sec_w);
    }
    if(time_w == 0 || ff_w == 1){
        sprintf(buffer, "00:00 W  B %02u:%02uBLACK WIN", min_b, sec_b);
    }
    if(draw == 1){
        sprintf(buffer, "%02u:%02u W  B %02u:%02uDRAW", min_w, sec_w, min_b, sec_b);
    }

    LCD_ClearScreen();
    LCD_PutString(buffer, strlen(buffer));
    __delay_ms(2500);
    return 0;
}