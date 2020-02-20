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

#define T_HOLD_BRIGHT   197
#define T_HOLD_DULL     108
    
#define LED_BLINK_2HZ_DELAY 31  // 500ms/16ms
#define LED_BLINK_2HZ_1_TIMES   2   // 2n
#define LED_BLINK_2HZ_2_TIMES   4   // 2n
#define LED_BLINK_2HZ_3_TIMES   6   // 2n
#define LED_BLINK_2HZ_4_TIMES   8   // 2n
#define LED_BLINK_2HZ_5_TIMES   10  // 2n
    
#define SW_POWER_DELAY 19   // 300ms/16ms
#define SW_MODE_DELAY 19    // 300ms/16ms
#define SW_MODE_RESET_DELAY 312     // 5000ms/16ms  

#define PWM_DUTY_DEFAULT 160    // 50% ==> 160/320    
#define PWM_MODE_DEFAULT 0
    
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