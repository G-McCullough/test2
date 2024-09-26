#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h" 
#include "Init.h"

/*************************************************************************************************** 
Conv_ASCII

Convert interger value to ascii character

Requirements
    None

Parameters
    None

Return Value
 dcValue; pulseASCII
***************************************************************************************************/
void Conv_ASCII (int time, char *pulseASCII)
{
    int sign;
    
    sign = time;
    if (sign<0)         /*record sign of number*/
    {
        time = -time; /*make time a positive number*/
    }
    do                  /*generate digits in reverse order*/
    {
        *pulseASCII=time%10+'0';   /*get next digit*/
        pulseASCII++;
    }
    while ((time/=10)>0);          /*delete it*/
    if (sign<0)         /*return sign if necessary*/
    {
        *pulseASCII='-';
        pulseASCII++;
    }
}

/*************************************************************************************************** 
Reverse_Digits

 Reverse the order of the ascii character digits

Requirements
    None

Parameters
    None

Return Value
 False
***************************************************************************************************/
void Reverse_Digits(char *revASCII)
{
    char tempsf[8];  /* 3 character buffer, increase size if more than 3 digits expected*/
                                       /* really should be a constant at top of code but left as literal make code easy to read*/
    char *tempsr = revASCII;  /*Using a temporary pointer we can change it but always have the original, s, point to start of the string.*/ 
    int i;
    int j;

    /* Reverse the sting into a temporary buffer */
    j = 0;
    for (i = (8-1);  0 < i;  i--) /* 3 character buffer, increase size if more than 3 digits expected*/
    {
        if (*tempsr != (char) 0) /* Ignore the null character */
        {
                tempsf[j] = *tempsr;
                j++;
                tempsr++;
        }
    }
    /* Now copy the string back to the original parameter. */
    if (8 > j) /* Check to make sure the string to reverse fits in the buffer.*/
    {
        while (8  > j) /* 3 character buffer, increase size if more than 3 digits expected*/
        {
                tempsf[j] = (char)0;
                j++;
        }
    }
    for (i = (8-1);  i >= 0;   i--) /* String is now in correct order, copy back to the original parameter. */
    {
                *revASCII = tempsf[i];
                revASCII++;
    }
}
