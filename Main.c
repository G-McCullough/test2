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
void Clock(void);
void LCD_Cursor_Off(void);
void LCD_Clear(void);
void TMR2_Setup(void);
void initialise (void);
void test_Setup1(void);
void test_Setup2(void);
void powerOff(void);
//void readPins(void);

int timerValue_ms = 0;
int timerValue_s = 0;
int timerValue_m = 0;
int timerValue_h = 0;
int position_x = 0;
int position_y = 0;
int mode = 0;
int primary = 0;
int secondary = 0;
int primaryOn = 0;
int secondaryOn = 0;
int speedOn = 0;
int releaseTrigger = 0;
char asciiInteger[8];
char dipSw = 0x00;
int cycleTime = 0;
int speedkeyChoice = 0;
/*******************************************************************************/
void initialise ()
{
OSCCON = 0x1EA; //500KHz
Port_Dir_Ini();
Port_Init_State();
Eusart_Setup();
Disp_Size(); /* For 20 by 4 LCD digole display only*/
LCD_Cursor_Off();
Delay(1000);
LCD_Clear();
Delay(1000);
LCD_Text_Position_xy(0,0);
#ifdef ODCFW40442
    Eusart_Transmit("ODC:FT:3598 Burn In");
#endif
#ifdef ODCFW40441
    Eusart_Transmit("ODC:FT:3597 Burn In");
#endif
LCD_Text_Position_xy(0,1);
Eusart_Transmit("Timer    HH:MM:SS");
LCD_Text_Position_xy(0,3);
Eusart_Transmit("Mode:");
LCD_Text_Position_xy(6,3);
Eusart_Transmit("Press Start");
Delay(1000);
TMR2_Setup();
}

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
void main(void) 
{    
    Delay(4000);
    initialise();
    CLRWDT();
    MODE_RELAY = 0;
    PSU_ON = 1;
    mode = 0; 

    while(1)
    {
        CLRWDT();
        if(TIME_START)
        {
            while(TIME_START)
            {
                CLRWDT();
            }
            releaseTrigger = 1;
        }
        while(!TIME_START && releaseTrigger == 1)
        {
            if(primaryOn == 0)
            {
                LCD_Clear();
                test_Setup1();
                Delay(10000);
                primaryOn = 1;
                break;
            }
            if(secondaryOn == 0)
            {
                LCD_Clear();
                test_Setup2();
                Delay(10000);
                secondaryOn = 1;
                break;
            }
            if(speedOn == 0)
            {
                LCD_Clear();
                LCD_Text_Position_xy(1,1);
                Eusart_Transmit("Pick Speed Of Test");
                LCD_Text_Position_xy(1,2);
                Eusart_Transmit("Key to Pos 1 or 2");
                speedOn = 1;
                break;
            }
            
            if(PORTCbits.RC7 == 0)
            {
             speedkeyChoice =  1;
            }
            
            LCD_Clear();
            LCD_Text_Position_xy(0,0);
            #ifdef ODCFW40442
                Eusart_Transmit("ODC:FT:3598 Burn In");
            #endif
            #ifdef ODCFW40441
                Eusart_Transmit("ODC:FT:3597 Burn In");
            #endif
            LCD_Text_Position_xy(0,1);
            Eusart_Transmit("Timer    HH:MM:SS");
            //LCD_Text_Position_xy(0,3);
            //Eusart_Transmit("Mode:");
            //LCD_Text_Position_xy(6,3);
            //Eusart_Transmit("OFF          0");
            /*Turn Lights off after test*/
            powerOff();
            for(;;) /*Start main loop of program*/
            {
                if (mode == 15)
                {
                    break;
                }
                CLRWDT();
                if (TMR2IF)
                    {
                    timerValue_ms ++;
                    TMR2IF = 0; 
                    MILLISECONDS = 1;
                    Clock();
                    }
                Mode_Select();
                MILLISECONDS = 0;
            }    
        }
        while(!TIME_START && releaseTrigger == 1)
        {
          CLRWDT(); 
        }
        releaseTrigger = 0;
    }
}

/*******************************************************************************/
void test_Setup1()
{ 
    /*Test primary light of DC Combi works */
    LCD_Text_Position_xy(6,1);
    Eusart_Transmit("PRIMARY");
    LCD_Text_Position_xy(8,2);
    Eusart_Transmit("TEST");
    PSU_ON = 0;
    MODE_RELAY = 0;
}

void test_Setup2()
{
   /*Test secondary light of DC combi works*/
    LCD_Text_Position_xy(5,1);
    Eusart_Transmit("SECONDARY");
    LCD_Text_Position_xy(8,2);
    Eusart_Transmit("TEST");
    PSU_ON = 0;
    MODE_RELAY = 1;
}

void powerOff()
{
    PSU_ON = 1;
    MODE_RELAY = 0;
}
void Clock()
{
    if( speedkeyChoice == 0)
    {
        if (timerValue_ms == 970) //This value has been adjusted to ensure 1mS is 1mS. Other functions within the code are taken approximately 30mS hence this adjustment. 
            {
                timerValue_s ++;
                timerValue_ms = 0;
                SECONDS = 1;
                Delay(100);
                LCD_Text_Position_xy(14,2);
                if (timerValue_s < 0x0A)
                    {
                        Eusart_Transmit(": ");
                        Conv_ASCII(timerValue_s,&asciiInteger[0]);
                        Reverse_Digits(&asciiInteger[0]);
                        Eusart_Transmit(&asciiInteger[7]);  
                    }
                    else if (timerValue_s == 0x3C)
                        {
                            Eusart_Transmit(":00");
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
            if (timerValue_s == 0x3C)
                {
                    timerValue_m ++;
                    timerValue_s = 0;
                    MINUTES = 1;
                    Delay(100);
                    LCD_Text_Position_xy(11,2);
                    if (timerValue_m < 0x0A)
                    {
                        Eusart_Transmit(": ");
                        Conv_ASCII(timerValue_m,&asciiInteger[0]);
                        Reverse_Digits(&asciiInteger[0]);
                        Eusart_Transmit(&asciiInteger[7]);  
                    }
                    else if (timerValue_m == 0x3C)
                    {
                        Eusart_Transmit(":00");
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
                if (timerValue_m == 0x3C)
                {
                    timerValue_h ++;
                    timerValue_m = 0;
                    LCD_Text_Position_xy(10,2);
                    Delay(100);
                    Conv_ASCII(timerValue_h,&asciiInteger[0]);
                    Reverse_Digits(&asciiInteger[0]);
                    Eusart_Transmit(&asciiInteger[7]);
                }
                if (timerValue_h == 8)
                {
                    timerValue_ms = 0;
                    timerValue_s = 0;
                    timerValue_m = 0;
                    timerValue_h = 8;
                    Delay(10000);
                }
    }
    else
    {
        if (timerValue_ms == 1) //This value has been adjusted to ensure 1mS is 1mS. Other functions within the code are taken approximately 30mS hence this adjustment. 
            {
                timerValue_s ++;
                timerValue_ms = 0;
                SECONDS = 1;
                Delay(100);
                LCD_Text_Position_xy(14,2);
                if (timerValue_s < 0x0A)
                    {
                        Eusart_Transmit(": ");
                        Conv_ASCII(timerValue_s,&asciiInteger[0]);
                        Reverse_Digits(&asciiInteger[0]);
                        Eusart_Transmit(&asciiInteger[7]);  
                    }
                    else if (timerValue_s == 0x3C)
                        {
                            Eusart_Transmit(":00");
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
            if (timerValue_s == 0x3C)
                {
                    timerValue_m ++;
                    timerValue_s = 0;
                    MINUTES = 1;
                    Delay(100);
                    LCD_Text_Position_xy(11,2);
                    if (timerValue_m < 0x0A)
                    {
                        Eusart_Transmit(": ");
                        Conv_ASCII(timerValue_m,&asciiInteger[0]);
                        Reverse_Digits(&asciiInteger[0]);
                        Eusart_Transmit(&asciiInteger[7]);  
                    }
                    else if (timerValue_m == 0x3C)
                    {
                        Eusart_Transmit(":00");
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
                if (timerValue_m == 0x3C)
                {
                    timerValue_h ++;
                    timerValue_m = 0;
                    LCD_Text_Position_xy(10,2);
                    Delay(100);
                    Conv_ASCII(timerValue_h,&asciiInteger[0]);
                    Reverse_Digits(&asciiInteger[0]);
                    Eusart_Transmit(&asciiInteger[7]);
                }
                if (timerValue_h == 8)
                {
                    timerValue_ms = 0;
                    timerValue_s = 0;
                    timerValue_m = 0;
                    timerValue_h = 8;
                    Delay(10000);
                }
    }
}
/*******************************************************************************/
void Mode_Select ()
{
    #ifdef ODCFW40442
        if ((mode == 0) && (timerValue_h == 0) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF          0"); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 1;
        }
        if ((mode == 1) && (timerValue_h == 1) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("WHITE ON     1"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 2;
        }
        if ((mode == 2) && (timerValue_h == 1) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF          2"); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 3;
        }
        if ((mode == 3) && (timerValue_h == 1) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("WHITE ON     3"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 4;
        }
        if ((mode == 4) && (timerValue_h == 3) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF          4"); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 5;
        }
        if ((mode == 5) && (timerValue_h == 3) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("WHITE ON     5"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 6;
        }
        if ((mode == 6) && (timerValue_h == 3) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF          6"); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 7;
        }
        if ((mode == 7) && (timerValue_h == 4) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF          7"); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 8;
        }
        if ((mode == 8) && (timerValue_h == 5) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("SECONDARY ON 8"); 
            PSU_ON = 0;
            MODE_RELAY = 1;
            mode = 9;
        }
        if ((mode == 9) && (timerValue_h == 5) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF          9"); 
            PSU_ON = 1;
            MODE_RELAY = 1;
            mode = 10;
        }
        if ((mode == 10) && (timerValue_h == 5) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("SECONDARY ON 10"); 
            PSU_ON = 0;
            MODE_RELAY = 1;
            mode = 11;
        }
        if ((mode == 11) && (timerValue_h == 7) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit(" OFF         11"); 
            PSU_ON = 1;
            MODE_RELAY = 1;
            mode = 12;
        }
        if ((mode == 12) && (timerValue_h == 7) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("SECONDARY ON 12"); 
            PSU_ON = 0;
            MODE_RELAY = 1;
            mode = 13;
        }
        if ((mode == 13) && (timerValue_h == 7) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit(" OFF         13"); 
            PSU_ON = 1;
            MODE_RELAY = 1;
            mode = 14;
        }
        if ((mode == 14) && (timerValue_h == 8) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit(" COMPLETE    14"); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 15;
        }
        if (mode > 15)
        {
            mode = 0;
        }
    #endif
    #ifdef ODCFW40441
        if ((mode == 0) && (timerValue_h == 0) && (timerValue_m == 0) && (timerValue_s == 1))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 1;
        }
        if ((mode == 1) && (timerValue_h == 1) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("PRIMARY ON"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 2;
        }
        if ((mode == 2) && (timerValue_h == 1) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 3;
        }
        if ((mode == 3) && (timerValue_h == 1) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("PRIMARY ON"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 4;
        }
        if ((mode == 4) && (timerValue_h == 3) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 5;
        }
        if ((mode == 5) && (timerValue_h == 3) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("PRIMARY ON"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 6;
        }
        if ((mode == 6) && (timerValue_h == 3) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 7;
        }
        if ((mode == 7) && (timerValue_h == 4) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 8;
        }
        if ((mode == 8) && (timerValue_h == 5) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("PRIMARY ON"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 9;
        }
        if ((mode == 9) && (timerValue_h == 5) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(6,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 10;
        }
        if ((mode == 10) && (timerValue_h == 5) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("PRIMARY ON"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 11;
        }
        if ((mode == 11) && (timerValue_h == 7) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 12;
        }
        if ((mode == 12) && (timerValue_h == 7) && (timerValue_m == 20) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("PRIMARY ON"); 
            PSU_ON = 0;
            MODE_RELAY = 0;
            mode = 13;
        }
        if ((mode == 13) && (timerValue_h == 7) && (timerValue_m == 30) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("OFF           "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 14;
        }
        if ((mode == 14) && (timerValue_h == 8) && (timerValue_m == 0) && (timerValue_s == 0))
        {
            LCD_Text_Position_xy(5,3);
            Eusart_Transmit("COMPLETE      "); 
            PSU_ON = 1;
            MODE_RELAY = 0;
            mode = 15;
        }
        if (mode > 15)
        {
            mode = 0;
        }
    #endif
}




