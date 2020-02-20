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

#include "../include/tcb0.h"
#include "../include/pin_manager.h"
#include "../mcc.h"

volatile int16_t i16Cnt = 0;
volatile uint16_t u16PwmDutyTemp = 0;
volatile uint16_t u16PwmDutyStep = 0;
volatile uint16_t u16PwmDutyTarget = PWM_DUTY_DEFAULT;
volatile uint16_t u16PwmState = PWM_OFF;
volatile uint16_t u16PwmMode = PWM_MODE_DEFAULT;

/**
 * \brief Initialize tcb interface
 *
 * \return Initialization status.
 */

void (*TCB0_CAPT_isr_cb)(void) = NULL;

void TCB0_SetCaptIsrCallback(TCB0_cb_t cb)
{
    TCB0_CAPT_isr_cb = cb; 
}

ISR(TCB0_INT_vect)
{
	/* Insert your TCB interrupt handling code */
    
#if 1
    /* LED Indicator */
    if(LED_Struct_t.u8PreLedState != LED_Struct_t.u8LedState)
    {
        LED_Struct_t.u8Times = 0;
    }
    
    switch (LED_Struct_t.u8LedState)
    {
        case LED_OFF:
            IO_PC0_SetHigh();
            IO_PC1_SetHigh();
            LED_Struct_t.u8Times = 0;
            break;
        case LED_GREEN_SOLID:
            IO_PC0_SetHigh();
            IO_PC1_SetLow();
            LED_Struct_t.u8Times = 0;
            break;
        case LED_GREEN_BLINK_2HZ:
            if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
            {
                LED_Struct_t.u8Delay++;
            }
            else
            {
                LED_Struct_t.u8Delay = 0;
                IO_PC0_SetHigh();
                IO_PC1_Toggle();                
            }
            LED_Struct_t.u8Times = 0;
            break;
        case LED_GREEN_BLINK_2HZ_1_TIMES:
            if(LED_Struct_t.u8Times < LED_BLINK_2HZ_1_TIMES)
            {
                if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
                {
                    LED_Struct_t.u8Delay++;
                }
                else
                {
                    LED_Struct_t.u8Delay = 0;
                    LED_Struct_t.u8Times++;               
                }
                if((LED_Struct_t.u8Times & 0x01) == 0)
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();                    
                }
                else
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetLow();                    
                }
            }
            else
            {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();
                    //LED_Struct_t.u8Times = 0;
            }
            break;
        case LED_GREEN_BLINK_2HZ_2_TIMES:
            if(LED_Struct_t.u8Times < LED_BLINK_2HZ_2_TIMES)
            {
                if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
                {
                    LED_Struct_t.u8Delay++;
                }
                else
                {
                    LED_Struct_t.u8Delay = 0;
                    LED_Struct_t.u8Times++;               
                }
                if((LED_Struct_t.u8Times & 0x01) == 0)
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();                    
                }
                else
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetLow();                    
                }
            }
            else
            {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();
                    //LED_Struct_t.u8Times = 0;
            }            
            break;
        case LED_GREEN_BLINK_2HZ_3_TIMES:
            if(LED_Struct_t.u8Times < LED_BLINK_2HZ_3_TIMES)
            {
                if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
                {
                    LED_Struct_t.u8Delay++;
                }
                else
                {
                    LED_Struct_t.u8Delay = 0;
                    LED_Struct_t.u8Times++;               
                }
                if((LED_Struct_t.u8Times & 0x01) == 0)
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();                    
                }
                else
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetLow();   
                }
            }
            else
            {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();
                    //LED_Struct_t.u8Times = 0;
            }            
            break;
        case LED_GREEN_BLINK_2HZ_4_TIMES:
            if(LED_Struct_t.u8Times < LED_BLINK_2HZ_4_TIMES)
            {
                if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
                {
                    LED_Struct_t.u8Delay++;
                }
                else
                {
                    LED_Struct_t.u8Delay = 0;
                    LED_Struct_t.u8Times++;               
                }
                if((LED_Struct_t.u8Times & 0x01) == 0)
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();                    
                }
                else
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetLow();                    
                }
            }
            else
            {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();
                    //LED_Struct_t.u8Times = 0;
            }            
            break;
        case LED_GREEN_BLINK_2HZ_5_TIMES:
            if(LED_Struct_t.u8Times < LED_BLINK_2HZ_5_TIMES)
            {
                if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
                {
                    LED_Struct_t.u8Delay++;
                }
                else
                {
                    LED_Struct_t.u8Delay = 0;
                    LED_Struct_t.u8Times++;               
                }
                if((LED_Struct_t.u8Times & 0x01) == 0)
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();                    
                }
                else
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetLow();                    
                }
            }
            else
            {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();
                    //LED_Struct_t.u8Times = 0;
            }            
            break;
        case LED_ORANGE_SOLID:
            IO_PC0_SetLow();
            IO_PC1_SetLow();
            LED_Struct_t.u8Times = 0;
            break;
        case LED_ORANGE_BLINK_2HZ_1_TIMES:
            if(LED_Struct_t.u8Times < LED_BLINK_2HZ_1_TIMES)
            {
                if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
                {
                    LED_Struct_t.u8Delay++;
                }
                else
                {
                    LED_Struct_t.u8Delay = 0;
                    LED_Struct_t.u8Times++;               
                }
                if((LED_Struct_t.u8Times & 0x01) == 0)
                {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();                    
                }
                else
                {
                    IO_PC0_SetLow();
                    IO_PC1_SetLow();                    
                }
            }
            else
            {
                    IO_PC0_SetHigh();
                    IO_PC1_SetHigh();
                    //LED_Struct_t.u8Times = 0;
            }            
            break;
        case LED_RED_SOLID:
            IO_PC0_SetLow();
            IO_PC1_SetHigh();
            LED_Struct_t.u8Times = 0;
            break;
        case LED_RED_BLINK_2HZ:
            if(LED_Struct_t.u8Delay < LED_BLINK_2HZ_DELAY)
            {
                LED_Struct_t.u8Delay++;
            }
            else
            {
                LED_Struct_t.u8Delay = 0;
                IO_PC0_Toggle();
                IO_PC1_SetHigh();                
            }
            LED_Struct_t.u8Times = 0;            
            break;
        default:
            break;
    }
    LED_Struct_t.u8PreLedState = LED_Struct_t.u8LedState; 
#endif
    
#if 1
    /* PB Switch Detection */
    
    // Power Switch
    if(SW_POPWER_GetValue() != 0)
    {
        if(SW_Struct_t.u8SwPowerDelay < SW_POWER_DELAY)
        {
            SW_Struct_t.u8SwPowerDelay++;
        }
        else
        {
            SW_Struct_t.bSWPowerDelay = true;
        }
    }
    else
    {
        SW_Struct_t.u8SwPowerDelay = 0;
        if(SW_Struct_t.bSWPowerDelay == true)
        {
            SW_Struct_t.bSWPower ^= 1;
        }
        SW_Struct_t.bSWPowerDelay = false;
    }
    
    // Mode Switch
    if(SW_MODE_GetValue() != 0)
    {
        if(SW_Struct_t.u8SwModeDelay < SW_MODE_DELAY)
        {
            SW_Struct_t.u8SwModeDelay++;
        }
        else
        {
            SW_Struct_t.bSWModeDelay = true;
        }
        
        //
        if(System_State_Struc_t.u8MainState == SYSTEM_POWER_ON)
        {
            if(SW_Struct_t.u16SwModeResetDelay < SW_MODE_RESET_DELAY)
            {
                SW_Struct_t.u16SwModeResetDelay++; 
            }
            else
            {
                
            }
        }
    }
    else
    {
        SW_Struct_t.u8SwModeDelay = 0;
        if(SW_Struct_t.bSWModeDelay == true)
        {
            SW_Struct_t.bSWMode = true;
        }
        SW_Struct_t.bSWModeDelay = false;
    }
#endif
    
#if 1
    /* Motor Control Function */
    
    if(SW_Struct_t.bSWPower == true)
    {
        switch (u16PwmState)
        {
            case PWM_RAMP_ON:
                u16PwmDutyTemp = 0;
                u16PwmDutyStep = u16PwmDutyTarget >> 3;
                u16PwmState = PWM_RAMP_ON_DELAY;
                break;
            case PWM_RAMP_ON_DELAY:
                if(u16PwmDutyTemp > u16PwmDutyTarget)
                {
                    u16PwmDutyTemp = u16PwmDutyTarget;
                    u16PwmState = PWM_ON;
                }
                else
                {
                    u16PwmDutyTemp = u16PwmDutyTemp + u16PwmDutyStep;
                    //TCA0.SINGLE.CMP1 = u16PwmDutyTemp;
                }
                TCA0.SINGLE.CMP1 = u16PwmDutyTemp;
                break;
            case PWM_ON:
                TCA0.SINGLE.CMP1 = u16PwmDutyTemp;
                u16PwmState = PWM_ON_DELAY;
                i16Cnt = 0;
                break;
            case PWM_ON_DELAY:
                if(i16Cnt > T_HOLD_BRIGHT)
                {
                    u16PwmState = PWM_RAMP_OFF;
                    i16Cnt = 0;
                }
                else
                {
                    i16Cnt++;
                }
                break;
            case PWM_RAMP_OFF:
                u16PwmDutyTemp = u16PwmDutyTarget;
                u16PwmDutyStep = u16PwmDutyTarget >> 3;
                u16PwmState = PWM_RAMP_OFF_DELAY;           
                break;
            case PWM_RAMP_OFF_DELAY:
                if(u16PwmDutyTemp > u16PwmDutyStep)
                {
                    u16PwmDutyTemp = u16PwmDutyTemp - u16PwmDutyStep;
                    //TCA0.SINGLE.CMP1 = u16PwmDutyTemp;             
                }
                else
                {
                    u16PwmDutyTemp = 0;
                    u16PwmState = PWM_OFF;
                }
                TCA0.SINGLE.CMP1 = u16PwmDutyTemp;
                break;
            case PWM_OFF:
                TCA0.SINGLE.CMP1 = 0x00;        // set PWM duty = 0;
                u16PwmState = PWM_OFF_DELAY;
                i16Cnt = 0;
                break;
            case PWM_OFF_DELAY:
                if(i16Cnt > T_HOLD_DULL)
                {
                   u16PwmState =  PWM_RAMP_ON;
                   i16Cnt = 0;
                }
                else
                {
                    i16Cnt++;
                }
                break;                
            default:
                break;
        }
    }
    else
    {
        TCA0.SINGLE.CMP1 = 0x00;        // set PWM duty = 0;
        u16PwmState = PWM_RAMP_ON;
    }
#endif    
	/**
	 * The interrupt flag is cleared by writing 1 to it, or when the Capture register
	 * is read in Capture mode
	 */
    
	if (TCB0_CAPT_isr_cb != NULL)
        {
            (*TCB0_CAPT_isr_cb)();
        } 
    
	TCB0.INTFLAGS = TCB_CAPT_bm;
}

/**
 * \brief Initialize TCB interface
 */
int8_t TCB0_Initialize()
{
    //Compare or Capture
    TCB0.CCMP = 0x1FF;

    //Count
    TCB0.CNT = 0x00;

    //ASYNC disabled; CCMPINIT disabled; CCMPEN disabled; CNTMODE INT; 
    TCB0.CTRLB = 0x00;

    //DBGRUN disabled; 
    TCB0.DBGCTRL = 0x00;

    //FILTER disabled; EDGE disabled; CAPTEI disabled; 
    TCB0.EVCTRL = 0x00;

    //CAPT enabled; 
    TCB0.INTCTRL = 0x01;

    //CAPT disabled; 
    TCB0.INTFLAGS = 0x00;

    //Temporary Value
    TCB0.TEMP = 0x00;

    //RUNSTDBY disabled; SYNCUPD disabled; CLKSEL CLKDIV1; ENABLE enabled; 
    TCB0.CTRLA = 0x01;

    return 0;
}

void TCB0_WriteTimer(uint16_t timerVal)
{
	TCB0.CNT=timerVal;
}

uint16_t TCB0_ReadTimer(void)
{
	uint16_t readVal;

	readVal = TCB0.CNT;

	return readVal;
}

void TCB0_EnableInterrupt(void)
{
	TCB0.INTCTRL |= TCB_CAPT_bm; /* Capture or Timeout: enabled */
}

void TCB0_DisableInterrupt(void)
{
	TCB0.INTCTRL &= ~TCB_CAPT_bm; /* Capture or Timeout: disabled */

}

inline void TCB0_ClearInterruptFlag(void)
{
	TCB0.INTFLAGS &= ~TCB_CAPT_bm;

}

inline bool TCB0_IsInterruptEnabled(void)
{
        return ((TCB0.INTCTRL & TCB_CAPT_bm) > 0);
}
