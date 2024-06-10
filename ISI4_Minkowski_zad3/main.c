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

#define LCD_CLEAR       0x01    // 0b 0000 0001
#define LCD_HOME        0x02    // 0b 0000 0010
#define LCD_ON          0x0C    // 0b 0000 1100
#define LCD_OFF         0x08    // 0b 0000 1000
#define LCD_CONFIG      0x38    // 0b 0011 1000
#define LCD_CURSOR      0x80    // 0b 1000 0000
#define LINE1           0x00    // 0b 0000 0000
#define LINE2           0x40    // 0b 0000 0100
#define LCD_CUST_CHAR   0x40    // 0b 0000 0100
#define LCD_SHIFT_R     0x1D
#define LCD_SHIFT_L     0x1B

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

void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char address;
    if (row == 1){
        address = LCD_CURSOR + LINE1 + col;
    }
    if (row == 2){
        address = LCD_CURSOR + LINE2 + col;
    }
    LCD_sendCommand(address);
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

void LCD_saveCustChar(unsigned char slot, unsigned char *array){
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + slot * 8);
    for(i = 0; i < 8; i++){
        LCD_sendData(array[i]);
    }
}
unsigned char afar00[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00111,
	0b11111,
	0b00000,
	0b00000
};
unsigned char afar10[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11100,
	0b00000,
	0b01111
};
unsigned char afar20[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b00111,
	0b00000,
	0b00000
};
unsigned char afar30[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11100,
	0b11111,
	0b00000,
	0b00000
};
unsigned char afar01[8] = {
	0b01111,
	0b00011,
	0b11111,
	0b11011,
	0b01111,
	0b00000,
	0b11111,
	0b00111
};
unsigned char afar11[8] = {
	0b11111,
	0b01100,
	0b01100,
	0b01100,
	0b01100,
	0b00000,
	0b11100,
	0b11111
};
unsigned char afar21[8] = {
	0b01111,
	0b00011,
	0b11111,
	0b11011,
	0b01111,
	0b00000,
	0b00111,
	0b11111
};
unsigned char afar31[8] = {
	0b11111,
	0b11011,
	0b11000,
	0b11000,
	0b11000,
	0b00000,
	0b11111,
	0b11100
};

void logo(char* top, char* bottom, unsigned int p){ // pozycja 6 + TEKST
    LCD_setCursor(1, p);
    LCD_sendData(0);
    LCD_sendData(1);
    LCD_sendData(2);
    LCD_sendData(3);
    LCD_print(" ");
    LCD_print(top);
    LCD_print(" ");
    LCD_setCursor(2, p);
    LCD_sendData(4);
    LCD_sendData(5);
    LCD_sendData(6);
    LCD_sendData(7);
    LCD_print(" ");
    LCD_print(bottom);
    LCD_print(" ");
}

int main(void) {
    unsigned char i;
    TRISB = 0x7FFF;
    TRISD = 0x0000;
    TRISE = 0x0000;
    
    LCD_init();
    
    LCD_saveCustChar(0, afar00);
    LCD_saveCustChar(1, afar10);
    LCD_saveCustChar(2, afar20);
    LCD_saveCustChar(3, afar30);
    LCD_saveCustChar(4, afar01);
    LCD_saveCustChar(5, afar11);
    LCD_saveCustChar(6, afar21);
    LCD_saveCustChar(7, afar31);
    
    while(1){
        logo(" Hurtownia ", "Elektryczna", 0); 
        logo("Serdecznie", "Zapraszamy", 17); // 6 + 11 
        logo("", "", 33); // 17 + 6 + 10 
        __delay_ms(500);
        LCD_sendCommand(LCD_SHIFT_L);
    }
    return 0;
}