#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h"
#include "Init.h"

void Delay(long int time);

/*************************************************************************************************** 
Eusart_Setup

 Setup EUSART for 16 bit Asynchronous operation
 Device configured as a Master
 9600 Baud rate

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void Eusart_Setup (void)
{
    SYNC = 0;   /*ASynchronous operation*/
    BRG16 = 1;  /*16-bit ASynchronous operation*/
    CSRC = 1;   /*Set device as a Master*/
    SPEN = 1;   /*Enables the EUSART*/
    SREN = 0;   /*Place device in Trasmit mode*/
    CREN = 0;   /*Place device in Trasmit mode*/
    SPBRG = 25; /*9600 Baud Rate*/
}    


/*************************************************************************************************** 
Eusart_Transmit

Transmit data out of EUSART by writing relevant data to TX1REG. "TT" is required prior to any 
transmission to the Digole LCD Decoder

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void Eusart_Transmit (char *txData)
{
    int z;
    {
        for (z = 0; z < 2; z++) /*Loop to set display up to display text. Command TT*/
        {
            TXEN = 1;
            TX1REG = ('T');
            Delay(20);
        }
        for (z = 0; z < (*txData); z++)
        {
            TX1REG = *txData;
            Delay(50);
            txData++;
        }
        TX1REG=0x00;
        Delay(50);
        }
}

