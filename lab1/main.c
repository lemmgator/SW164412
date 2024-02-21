#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BIT_VALUE(val, no_bit) (val >> no_bit) & 1

void delay()
{
    for(int a = 0; a <= 10000000; a++){
        for(int b = 0; b <= 500; b++){

        }
    }
}

int bit_value(int val, int no_bit)
{
    int osemka = pow(2, no_bit);
    if(val & osemka){return 1;}
    return 0;
}

int IntToGray(unsigned char input)
{
    return (input >> 1) ^ input;
}
int main()
{
    int count = 0;
    while(1){
        printf("\r");
        for(int i = 7; i >= 0; i--){
            printf("%d", bit_value(count, i));
        }
        printf("\t");
        for(int i = 7; i >= 0; i--){
            printf("%d", bit_value(IntToGray(count), i));
        }
        count++;
        delay();
    }

    return 0;
}
