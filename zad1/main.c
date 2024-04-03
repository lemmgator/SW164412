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

unsigned portValue = 0, bcdCounter = 0, bcdValue = 0;
char prevS6 = 0, currentS6 = 0, prevS7 = 0, currentS7 = 0, program = 0;

void bcd99up(){
    bcdValue = 0;

    if(portValue % 9 == 0){
        bcdValue++;
        portValue = 0;
    }
    if(bcdCounter % 2 == 0){
        bcdValue += 10;
        if(bcdCounter % 4 == 0){
            bcdValue += 100;
            if(bcdCounter % 8 == 0){
                bcdValue += 1000;
            } 
        } 
    } 
    LATA = portValue + bcdValue;
}

void bcd99down(){
    portValue--;
}

void snake(){
    portValue++;
    LATA = (portValue>>2);
}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    if(program == 0){
        portValue++;
        LATA = portValue;
    }
    if(program == 1){
        portValue--;
        LATA = portValue;
    }
    if(program == 2){
        portValue++;
        LATA = (portValue>>1)^portValue;
    }
    if(program == 3){
        portValue--;
        LATA = (portValue<<1)^portValue;
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
            bcdCounter = 0;
            portValue = 0;
            program--;
        }
        if (currentS7 - prevS7 == -1){
            bcdCounter = 0;
            portValue = 0;
            program++;
        }
    }
    
    return 0;
}
