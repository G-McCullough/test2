#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h" 
#include "Init.h"

void Delay(long int time);
void LCD_Text_Position_xy (int position_x, int position_y);
//void Conv_ASCII (int dcValue, char *pwmDutyCycleASCII);
//void Reverse_Digits(char *revASCII);

void Clock (void)
{


if (TMR2IF)
    {
        timerValue_ms ++;
        TMR2IF = 0; 
        MILLISECONDS = 1;
    }
MILLISECONDS = 0;
            
if (timerValue_ms == 1000)
    {
    timerValue_s ++;
    timerValue_ms = 0;
    SECONDS = 1;
    Delay(100);
    LCD_Text_Position_xy(11,2);
    Eusart_Transmit(": ");
    Conv_ASCII(timerValue_s,&asciiInteger[0]);
    Reverse_Digits(&asciiInteger[0]);
    Eusart_Transmit(&asciiInteger[7]);
    }
SECONDS = 0;
            
if (timerValue_s == 60)
    {
    timerValue_m ++;
    timerValue_s = 0;
    MINUTES = 1;
    Delay(100);
    LCD_Text_Position_xy(8,2);
    Eusart_Transmit(": ");
    Conv_ASCII(timerValue_m,&asciiInteger[0]);
    Reverse_Digits(&asciiInteger[0]);
    Eusart_Transmit(&asciiInteger[7]);
    }
MINUTES = 0;
            
if (timerValue_m == 5)
    {
    timerValue_h ++;
    timerValue_m = 0;
    LCD_Text_Position_xy(7,2);
    Conv_ASCII(timerValue_h,&asciiInteger[0]);
    Reverse_Digits(&asciiInteger[0]);
    Eusart_Transmit(&asciiInteger[7]);
    }

if (timerValue_h == 24)
    {
    timerValue_h = 0;
    }
}