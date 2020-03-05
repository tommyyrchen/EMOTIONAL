/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef TCB0_H_INCLUDED
#define TCB0_H_INCLUDED

#include "../utils/compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define T_HOLD_BRIGHT   108 //1728ms/16ms
#define T_HOLD_DULL     197 //3152ms/16ms
#define SS_CNT          4    
    
#define LED_BLINK_2HZ_DELAY 31  // 500ms/16ms
#define LED_BLINK_2HZ_1_TIMES   2   // 2n
#define LED_BLINK_2HZ_2_TIMES   4   // 2n
#define LED_BLINK_2HZ_3_TIMES   6   // 2n
#define LED_BLINK_2HZ_4_TIMES   8   // 2n
#define LED_BLINK_2HZ_5_TIMES   10  // 2n
    
#define SW_POWER_DELAY  9       // 150ms/16ms
#define SW_MODE_DELAY   9       // 150ms/16ms
#define SW_MODE_RESET_DELAY 312     // 5000ms/16ms
#define SW_MS_DELAY         312     // 5000ms/16ms
#define SW_MS_RESET_DELAY   1875    // 30000ms/16ms
#define BATTERY_LOW_DELAY   13      // 200ms/16ms
#define POWER_OFF_DELAY     112500  // 30 minutes = (30 x 60 x 1000)ms/16ms

#define SW_MS_COUNT		7
#define SW_MS_TIMEIOUT_COUNT	125	// 2000ms/16ms
#define SW_MS_OUT_COUNT			188 // 3000ms/16ms

#define PWM_DUTY_DEFAULT (1249)    // 50% ==> 1250/2500    
#define PWM_MODE_DEFAULT 0
#define PWM_DUTY_MIN        160
#define PWM_DUTY_MAX        320
    
volatile extern int16_t i16Cnt;
volatile extern uint16_t u16PwmDutyTemp;
volatile extern uint16_t u16PwmDutyStep;
volatile extern uint16_t u16PwmDutyTarget;

volatile extern uint16_t u16PwmState;
volatile extern uint16_t u16PwmMode;

enum PWM_CONTROL_NNUM
{
    PWM_RAMP_ON = 0,
    PWM_RAMP_ON_DELAY = 1,
    PWM_ON = 2,
    PWM_ON_DELAY = 3,
    PWM_RAMP_OFF = 4,
    PWM_RAMP_OFF_DELAY = 5,
    PWM_OFF = 6,
    PWM_OFF_DELAY = 7,    
}PWM_CONTROL;

enum LED_STATE_ENUM
{
    LED_OFF = 0,
    LED_GREEN_SOLID = 1,
    LED_GREEN_BLINK_2HZ = 2,
    LED_GREEN_BLINK_2HZ_1_TIMES = 3,
    LED_GREEN_BLINK_2HZ_2_TIMES = 4,
    LED_GREEN_BLINK_2HZ_3_TIMES = 5,
    LED_GREEN_BLINK_2HZ_4_TIMES = 6,
    LED_GREEN_BLINK_2HZ_5_TIMES = 7,    
    LED_ORANGE_SOLID = 8,
    LED_ORANGE_BLINK_2HZ_1_TIMES = 9,
    LED_RED_SOLID = 10,
    LED_RED_BLINK_2HZ = 11,
}LED_STATE;

struct SW_Struct
{
    volatile uint8_t u8SwPowerDelay;
    volatile uint8_t u8SwModeDelay;
    volatile bool bSWPower;
    volatile bool bSWPowerDelay;
    volatile bool bSWMode;
    volatile bool bSWModeDelay;
    volatile uint16_t u16SwModeResetDelay;
    volatile bool bSwModeResetDelay;
    volatile bool bSwModeReset;
    volatile uint16_t u16SwMSDelay;
    volatile bool bSwMSDelay;
    volatile bool bSwMS;
    volatile uint16_t u16SwMSResetDelay;
    volatile bool bSwMSReset;
	
	volatile uint8_t u8SwMSCount;
	volatile uint8_t u8SwMSCntTimeOut;
	volatile uint8_t u8SwMSOutCnt;
	volatile bool bSwMSOutCnt;
	volatile bool bSwMSOut;
}SW_Struct_t;

struct LED_Struct
{
    volatile uint8_t u8LedState;
    volatile uint8_t u8PreLedState;
    volatile uint8_t u8Delay;
    volatile uint8_t u8Times;
}LED_Struct_t;

typedef void (*TCB0_cb_t)(void);
void TCB0_SetCaptIsrCallback(TCB0_cb_t cb);

int8_t TCB0_Initialize();
void TCB0_EnableInterrupt(void);
void TCB0_DisableInterrupt(void);
uint16_t TCB0_ReadTimer(void);
void TCB0_WriteTimer(uint16_t timerVal);
void TCB0_ClearInterruptFlag(void);
bool TCB0_IsInterruptEnabled();

#ifdef __cplusplus
}
#endif

#endif /* TCB0_H_INCLUDED */