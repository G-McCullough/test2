#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h" 
#include "Init.h"


/*************************************************************************************************** 
TMR2_Setup

    TMR2 setup, see page 243 of PIC16F1619 Datasheet for register details. Interupt at 1mS

Requirements
    None

Parameters
    None

Return Value
 False
***************************************************************************************************/
void TMR2_Setup (void)
{
    T2CLKCON = 0x01;
    T2CON = 0x8f;
    TMR2IE = 1;
    PR2 = 0xfa;
}


