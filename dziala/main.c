#pragma config POSCMOD = NONE // XT Oscillator mode selected
#pragma config OSCIOFNC = OFF // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = FRC // Primary Oscillator (XT, HS, EC)
#pragma config IESO = OFF // Int Ext Switch Over Mode enabled

#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 //

#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled

#include "xc.h"
#include "libpic30.h"

#include <math.h>

unsigned portValue = 0, bcdCounter = 0, bcdValue = 0, i = 0, j = 0, dir = 0, k = 1;
char prevS6 = 0, currentS6 = 0, prevS7 = 0, currentS7 = 0, program = 0;

void binaryup(){
    portValue++;
    LATA = portValue;
}

void binarydown(){
    portValue--;
    LATA = portValue;
}

void grayup(){
    portValue++;
    LATA = portValue ^ (portValue>>1);
}

void graydown(){
    portValue--;
    LATA = portValue ^ (portValue>>1);
}

void bcd99up(){
    portValue++;
    if(portValue % 10 == 0){
        bcdCounter++;
        portValue = 0;
    }
    if(bcdCounter == 10){
        bcdCounter = 0;
        bcdValue = 0;
    }
    LATA = portValue | bcdCounter << 4;
}

void bcd99down(){
    portValue--;
    if(portValue == 0){
        bcdCounter--;
        portValue = 9;
    }
    
    if(bcdCounter == -1){
        bcdCounter = 9;
        bcdValue = 9;
        portValue = 9;
    }
    LATA = portValue | bcdCounter << 4;
}

void snake(){
    LATA = 7 << i;

    if(dir == 0) i++;
    if(dir == 1) i--;
    if(i == 5) dir = 1;
    if(i == 0) dir = 0;
}

void queue(){
    LATA = k + j;
    k = k << 1;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    if(program == 0){
        binaryup();
    }
    if(program == 1){
        binarydown();
    }
    if(program == 2){
        grayup();
    }
    if(program == 3){
        graydown();
    }
    if(program == 4){
        bcd99up();
    }
    if(program == 5){
        bcd99down();
    }
    if(program == 6){
        snake();
    }
    if(program == 7){
        queue();
    }
    _T1IF = 0;
}

int main(void) {
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    T1CON = 0x8010; // 0b10000000100000
    PR1 = 0xFFFF;
    _T1IE = 1;
    _T1IP = 1;
    while(1){
        prevS6 = PORTDbits.RD6;
        prevS7 = PORTDbits.RD7;
        __delay32(1500);
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
        if (currentS6 - prevS6 == -1){
            bcdValue = 0;
            bcdCounter = 0;
            portValue = 0;
            i = 0;
            j = 0;
            k = 0;
            program--;
            
            if(program == 5){
                bcdCounter = 9;
                bcdValue = 9;
                portValue = 9;
            }
            if(program < 0) program = 8;
        }
        if (currentS7 - prevS7 == -1){
            bcdCounter = 0;
            portValue = 0;
            bcdValue = 0;
            i = 0;
            j = 0;
            k = 0;
            program++;
            
            if(program == 5){
                bcdCounter = 9;
                bcdValue = 9;
                portValue = 9;
            }
            
            if(program > 8) program = 0;
        }
    }
    
    return 0;
}