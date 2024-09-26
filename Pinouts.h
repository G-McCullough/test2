/* 
 * File:   Pinouts.h
 * Author: markr
 *
 * Created on 14 December 2015, 15:24
 */

#ifndef PINOUTS_H
#define	PINOUTS_H

#include <xc.h>

/*Input Pin Definitions*/
/*PortB*/
#define RECIEVE         RB5 /*EUSART Receive pin*/
/*PortC*/
#define TIME_START      RC4 /*PWM Increase signal*/
#define PWM_DECREASE    RC3 /*PWM Decrease signal*/

/*Output Pin Definitions*/
/*PortA*/
#define SECONDS           RA2 
#define MINUTES           RA5 
/*PortB*/
#define TRANSMIT        RB7 /*EUSART Transmit pin*/
/*PortC*/
#define MILLISECONDS        RC5 /*PWM3 Output pin*/
#define MODE_RELAY          RC6
#define PSU_ON              RC1
#define C17_ON              RC2

#endif	/* PINOUTS_H */

