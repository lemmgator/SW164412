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
#include <libpic30.h>

#define FCY         4000000UL
#define LCD_E       LATDbits.LATD4
#define LCD_RW      LATDbits.LATD5
#define LCD_RS      LATBbits.LATB15
#define LCD_DATA    LATE

#define LCD_CLEAR   0x01    // 0b 0000 0001
#define LCD_HOME    0x02    // 0b 0000 0010
#define LCD_ON      0x0C    // 0b 0000 1100
#define LCD_OFF     0x08    // 0b 0000 1000
#define LCD_CONFIG  0x38    // 0b 0011 1000

void __delay_us(unsigned long us){
    __delay32(FCY/1000000*us);
}

void __delay_ms(unsigned long ms){
    __delay32(FCY/1000*ms);
}

void LCD_sendCommand(unsigned char command){
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    LCD_DATA = command;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_sendData(unsigned char data){
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E = 1;
    LCD_DATA = data;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_print(char* string){
    while(*string){
        LCD_sendData(*string++);
    }
}

void LCD_init(){
    __delay_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    __delay_us(50);
    LCD_sendCommand(LCD_ON);
    __delay_us(50);
    LCD_sendCommand(LCD_CLEAR);
    __delay_ms(2);
}

int main(void) {
    TRISB = 0x7FFF;
    TRISD = 0x0000;
    TRISE = 0x0000;
    
    LCD_init();
    LCD_print("lmaoooooo,,,, :3");
    
    return 0;
}
