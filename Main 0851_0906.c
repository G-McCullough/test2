/* 
 * File:   main.c
 * Author: markr
 *
 * Created on 14 December 2015, 15:13
 * Oscillator running at 4MHz
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h"
#include "Init.h"

void Port_Dir_Ini (void);
void Port_Init_State (void);
void Delay (long int time);
void Disp_Size (void);
void Eusart_Setup (void);
void Eusart_Transmit (char *txData);
void LCD_Text_Position_xy (int position_x, int position_y);
void Conv_ASCII (int time, char *timeASCII);
void Reverse_Digits(char *revASCII);
void Mode_Select (void);
/*************************************************************************************************** 
main
    The main body of the program - setup registers and LCD display

Requirements
    None

Parameters
    None

Return Value
    None
***************************************************************************************************/
int main(void) 
{
OSCCON = 0x1EA; //500KHz
        

int timerValue_ms = 0;
int timerValue_s = 0;
int timerValue_m = 0;
int timerValue_h = 0;
int position_x = 0;
int position_y = 0;

char asciiInteger[8];
char mode;
               
        Port_Dir_Ini();
        Port_Init_State();
        Eusart_Setup();
        Disp_Size(); /* For 20 by 4 LCD digole display only*/
        LCD_Cursor_Off();
        Delay(1000);
        LCD_Clear();
        Delay(1000);
        TMR2_Setup();
        LCD_Text_Position_xy(0,0);
        Eusart_Transmit("NP300   ODC:FT:3598");
        LCD_Text_Position_xy(0,1);
        Eusart_Transmit("Timer HH:MM:SS");
        LCD_Text_Position_xy(0,3);
        Eusart_Transmit("Mode:");
        
                                            
        for(;;) /*Start main loop of program*/
        {
            CLRWDT();
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
                if (timerValue_s < 0x0A)
                {
                    Eusart_Transmit(": ");
                    Conv_ASCII(timerValue_s,&asciiInteger[0]);
                    Reverse_Digits(&asciiInteger[0]);
                    Eusart_Transmit(&asciiInteger[7]);  
                }
                else
                {
                    Eusart_Transmit(":");
                    Conv_ASCII(timerValue_s,&asciiInteger[0]);
                    Reverse_Digits(&asciiInteger[0]);
                    Eusart_Transmit(&asciiInteger[6]);
                }
            }
            SECONDS = 0;
            
            if (timerValue_s == 60)
            {
                timerValue_m ++;
                timerValue_s = 0;
                MINUTES = 1;
                Delay(100);
                LCD_Text_Position_xy(8,2);
                if (timerValue_m < 0x0A)
                {
                    Eusart_Transmit(": ");
                    Conv_ASCII(timerValue_m,&asciiInteger[0]);
                    Reverse_Digits(&asciiInteger[0]);
                    Eusart_Transmit(&asciiInteger[7]);  
                }
                else
                {
                    Eusart_Transmit(":");
                    Conv_ASCII(timerValue_m,&asciiInteger[0]);
                    Reverse_Digits(&asciiInteger[0]);
                    Eusart_Transmit(&asciiInteger[6]);
                }
            }
            MINUTES = 0;
            
            if (timerValue_m == 60)
            {
                timerValue_h ++;
                timerValue_m = 0;
                LCD_Text_Position_xy(7,2);
                Delay(100);
                Conv_ASCII(timerValue_h,&asciiInteger[0]);
                Reverse_Digits(&asciiInteger[0]);
                Eusart_Transmit(&asciiInteger[7]);
            }
                                
            if (timerValue_h == 10)
            {
                timerValue_ms = 0;
                timerValue_s = 0;
                timerValue_m = 0;
                timerValue_h = 0;
                Delay(10000);
            }
            Mode_Select();
        }
//return 0;
}
void Mode_Select ()
{
    LCD_Text_Position_xy(6,3);
    Eusart_Transmit("OFF"); 
    return;
}




