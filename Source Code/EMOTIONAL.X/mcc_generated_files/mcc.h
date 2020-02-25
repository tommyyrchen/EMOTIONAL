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

#ifndef MCC_H
#define	MCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/compiler.h"
#include "include/pin_manager.h"
#include "include/adc0.h"
#include "include/cpuint.h"
#include "include/tcb0.h"
#include "include/tca0.h"
#include "config/clock_config.h"

enum SYSTEM_STATE_ENUM
{
    SYSTEM_INIT = 0,
    SYSTEM_POWER_OFF = 1,
    SYSTEM_POWER_ON = 2,
    SYSTEM_MODE_SETTING = 3,
    SYSTEM_POWER_LOW = 4,
}SYSTEM_STATE;

enum SYSTEM_SUB_STATE_ENUM
{
    SYSTEM_SUB_INIT = 0,
    SYSTEM_SUB_MODE_1 = 1,
    SYSTEM_SUB_MODE_2 = 2,
    SYSTEM_SUB_MODE_3 = 3,
    SYSTEM_SUB_MODE_4 = 4,
}SYSTEM_SUB_STATE;

struct System_State_StrucT
{
    uint8_t u8MainState;
    uint8_t u8SubState;
    uint8_t u8PreSubState;
    uint8_t u8SubMode;
    volatile bool bAdcStart;
    volatile uint16_t u16Battery;
    volatile bool bBatteryLow;
    volatile bool bBatteryUltraLow;
    volatile bool bPWM_ON;
    volatile uint8_t u8BatteryLowDelay;
}System_State_Struc_t;

#define BATTERY_LOW_VOLTAGE         2635    // (1.1/3.42) x 1024 x 8
#define BATTERY_ULTRA_LOW_VOLTAGE   2515    // (1.05/3.42) x 1024 x 8
/**
 * Initializes MCU, drivers and middleware in the project
**/
void SYSTEM_Initialize(void);
int8_t BOD_Initialize();
int8_t CLKCTRL_Initialize();
int8_t SLPCTRL_Initialize();
int8_t WDT_Initialize();

#ifdef __cplusplus
}
#endif
#endif	/* MCC_H */