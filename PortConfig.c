#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h"
#include "Init.h"

/*************************************************************************************************** 
Port_Dir_Ini

    Intial port direction setup on switch on

Requirements
    None

Parameters
    None

Return Value
    False
***************************************************************************************************//*portDirIni - Intial port direction setup on switch on.*/
void Port_Dir_Ini (void)
{
    TRISA = 0x00;       /*PORTA, All outputs*/
//    TRISB = 0x00;       /*PORTB, All outputs*/
    TRISB = 0x186A0;    /*PORTB, RB5 input, all other outputs*/
    TRISC = 0x18;       /*PORTC, RC4, RC3 input, all other outputs*/
}


/*************************************************************************************************** 
PWM3_Setup

    portInitState - Intial port status on switch on

Requirements
    None

Parameters
    None

Return Value
    False
***************************************************************************************************/
void Port_Init_State (void)
{
    LATA = 0;
    ANSELC = 0X00;
    //LATC = 0x18;
    //WPUC = 0x18;
    RB7PPS = 0x12;      /*Set RB7 as transmit Pin*/
}