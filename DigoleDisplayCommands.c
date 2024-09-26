#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h"
#include "Init.h"

void Delay(long int time);
/*************************************************************************************************** 
Disp_Size

    This function sets the Digole display decoder to work with a 20 by 4 LCD display

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/


void Disp_Size (void)
{
    char *dispSize = "STCR";
    int ds;
        for (ds = 0; ds < (*dispSize); ds++)
        {
            TXEN = 1;
            TX1REG = *dispSize;
            Delay(50);
            *dispSize++;
        }
    TX1REG = 0x14;
    Delay(50);
    TX1REG = 0x04;
    Delay(50);
    TX1REG = 0x80;
    Delay(50);
    TX1REG = 0xC0;
    Delay(50);
    TX1REG = 0x94;
    Delay(50);
    TX1REG = 0xD4;
    Delay(50);
    TX1REG = 0x00;
    
}


/*************************************************************************************************** 
LCD_Cursor_Off

    This function turns off the cursor on the LCD display via the Digole LCD decoder

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void LCD_Cursor_Off (void)
{
    char *cursor = "CS";
    int cu;
        for (cu = 0; cu < (*cursor); cu++)
        {
            TXEN = 1;
            TX1REG = *cursor;
            Delay(50);
            *cursor++;
        }
    TX1REG = 0x00;
}


/*************************************************************************************************** 
LCD_Clear

    This function clears the LCD display via the Digole LCD decoder

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void LCD_Clear (void)
{
    char *clear = "CL";
    int cl;
        for (cl = 0; cl < (*clear); cl++)
        {
            TXEN = 1;
            TX1REG = *clear;
            Delay(50);
            *clear++;
        }
}


/*************************************************************************************************** 
LCD_Carriage_Return

    This function initiates a carriage return on the LCD display via the Digole LCD decoder

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void LCD_Carriage_Return (void)
{
    char *carriageReturn = "TRT";
    int cr;
    for (cr = 0; cr < (*carriageReturn); cr++)
    {
        TXEN = 1;
        TX1REG = *carriageReturn;
        Delay(50);
        *carriageReturn++;
    }
}
/*************************************************************************************************** 
LCD_Text_Position

    This function sets the cursor position on the LCD display via the Digole LCD decoder

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void LCD_Text_Position_xy (int position_x, int position_y)
{
    char *textPosition = "TP";
    int pos;
    int xy;
    
    for (pos = 0; pos < (*textPosition); pos++)
        {
            TXEN = 1;
            TX1REG = *textPosition;
            Delay(50);
            *textPosition++;
        }

    TX1REG = position_x;
    Delay(50);
    TX1REG = position_y;
    Delay(50);
    TX1REG=0x00;
    Delay(100);
}