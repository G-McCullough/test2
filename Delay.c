#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Pinouts.h"
#include "Init.h"


/*************************************************************************************************** 
 * Delay

    This function creates a delay equal to 32uS per count

Requirements
    None

Parameters
    None

Return Value
    FALSE
***************************************************************************************************/
void Delay (long int time)
{
    long int t;
    {
    for (t = 0; t < time; t++)
        {
            RB6 = 1;
            RB6 = 0;
        }
    }
}
