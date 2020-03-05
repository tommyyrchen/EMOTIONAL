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
#include "../include/adc0.h"

volatile int16_t i16Cnt = 0;
volatile uint16_t u16PwmDutyTemp = 0;
volatile uint16_t u16PwmDutyStep = 0;
volatile uint16_t u16PwmDutyTarget = PWM_DUTY_DEFAULT;
volatile uint16_t u16PwmState = PWM_OFF;
volatile uint16_t u16PwmMode = PWM_MODE_DEFAULT;
volatile uint8_t u8Cnt = 0;

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
    //PORTB_OUTSET = 0x20;
#if 1
    /* Motor Control Function */
    
    if(System_State_Struc_t.bPWM_ON == true)
    {
        switch (u16PwmState)
        {
            case PWM_RAMP_ON:
                TCA0.SINGLE.CMP1 = PWM_DUTY_MIN;
                TCA0.SINGLE.CTRLB = 0x2B;
                u16PwmDutyTemp = 0;
                u16PwmDutyStep = u16PwmDutyTarget >> 2;
                u16PwmState = PWM_RAMP_ON_DELAY;
                
                u8Cnt = 0;
                break;
            case PWM_RAMP_ON_DELAY:
                if(u8Cnt < SS_CNT)
                {
                    u8Cnt++;
                    if(u16PwmDutyTemp < u16PwmDutyTarget)
                    {
                        u16PwmDutyTemp = u16PwmDutyTemp + u16PwmDutyStep;
                    }
                    else
                    {
                        u16PwmDutyTemp = u16PwmDutyTarget;
                    }
                }
                else
                {
                    u16PwmDutyTemp = u16PwmDutyTarget;
                    u16PwmState = PWM_ON;
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
                u16PwmDutyStep = u16PwmDutyTarget >> 2;
                u16PwmState = PWM_RAMP_OFF_DELAY;
                
                u8Cnt = 0;
                break;
            case PWM_RAMP_OFF_DELAY:
                if(u8Cnt < (SS_CNT-1))
                {
                    u8Cnt++;
                    if(u16PwmDutyTemp > u16PwmDutyStep)
                    {
                        u16PwmDutyTemp = u16PwmDutyTemp - u16PwmDutyStep;
                    }
                }
                else
                {
                    u16PwmDutyTemp = PWM_DUTY_MIN;
                    u16PwmState = PWM_OFF;                   
                }
                TCA0.SINGLE.CMP1 = u16PwmDutyTemp;
                break;
            case PWM_OFF:
//                PORTB_OUTSET = 0x20;
                TCA0.SINGLE.CMP1 = 0x00;        // set PWM duty = 0;
                TCA0.SINGLE.CTRLB = 0x00;       // disabled PWM function
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
        TCA0.SINGLE.CTRLB = 0x00;       // disabled PWM function        
        u16PwmState = PWM_RAMP_ON;
    }
#endif

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
	#if 0
    if(SW_POWER_GetValue() != 0 && SW_MODE_GetValue() != 0)
    {
        if(SW_Struct_t.u16SwMSDelay < SW_MS_DELAY)
        {
            SW_Struct_t.u16SwMSDelay++;
        }
        else
        {
            SW_Struct_t.bSwMSDelay = true;
        }
    }
    else
    {
        SW_Struct_t.u16SwMSDelay = 0;
        if(SW_Struct_t.bSwMSDelay == true)
        {
            SW_Struct_t.bSwMS = true;
        }
        SW_Struct_t.bSwMSDelay = false;
    }
    #endif
	
    // Power Switch
    if(SW_POWER_GetValue() != 0)
    {
        if(SW_Struct_t.u16SwPowerDelay < SW_POWER_DELAY)
        {
            SW_Struct_t.u16SwPowerDelay++;
        }
        else
        {
            SW_Struct_t.bSWPowerDelay = true;
        }
        
        SW_Struct_t.u16SwMSResetDelay = 0;
		SW_Struct_t.u16SwMSCntTimeOut = 0;

		if(System_State_Struc_t.u8MainState == SYSTEM_MODE_SETTING)
		{
			if(SW_Struct_t.u16SwMSOutCnt < SW_MS_OUT_COUNT)
			{
				SW_Struct_t.u16SwMSOutCnt++;
			}
			else
			{
				//SW_Struct_t.bSwMSOutCnt = true;
                //SW_Struct_t.bSwMSOutCnt = false;
                SW_Struct_t.u16SwMSOutCnt = 0;
                SW_Struct_t.bSwMSOut = true;
			}
		}
    }
    else
    {
        SW_Struct_t.u16SwPowerDelay = 0;
        if(SW_Struct_t.bSWPowerDelay == true)
        {
            //SW_Struct_t.bSWPower ^= 1;
            SW_Struct_t.bSWPower = true;
						
			// check for going to the SYSTEM_MODE_SETTING
			if (SW_Struct_t.u8SwMSCount <= SW_MS_COUNT)
			{
				SW_Struct_t.u8SwMSCount++;
			}
			else
			{
				SW_Struct_t.bSwMS = true;
			}			
        }
        SW_Struct_t.bSWPowerDelay = false;

//		if(SW_Struct_t.bSwMSOutCnt == true)
//		{
//			SW_Struct_t.bSwMSOutCnt = false;
//			SW_Struct_t.u8SwMSOutCnt = 0;
//			SW_Struct_t.bSwMSOut = true;
//		}
        //SW_Struct_t.u16SwModeResetDelay = 0;
        
        SW_Struct_t.u16SwMSOutCnt = 0;

        if (SW_Struct_t.u16SwMSCntTimeOut < SW_MS_TIMEIOUT_COUNT)
        {
            SW_Struct_t.u16SwMSCntTimeOut++;
        }
        else
        {
            SW_Struct_t.u8SwMSCount = 0;
            SW_Struct_t.u16SwMSCntTimeOut = 0;
        }        
    }
    
    // Mode Switch
    if(SW_MODE_GetValue() != 0)
    {
        if(SW_Struct_t.u16SwModeDelay < SW_MODE_DELAY)
        {
            SW_Struct_t.u16SwModeDelay++;
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
                SW_Struct_t.bSwModeResetDelay = true;
            }
        }
        
        SW_Struct_t.u16SwMSResetDelay = 0;
    }
    else
    {
        SW_Struct_t.u16SwModeDelay = 0;
        if(SW_Struct_t.bSWModeDelay == true)
        {
            SW_Struct_t.bSWMode = true;
        }
        SW_Struct_t.bSWModeDelay = false;
        
        SW_Struct_t.u16SwModeResetDelay = 0;
        if(SW_Struct_t.bSwModeResetDelay == true)
        {
            SW_Struct_t.bSwModeReset = true;
        }
        SW_Struct_t.bSwModeResetDelay = false;
        
        //SW_Struct_t.u16SwModeResetDelay = 0;
    }
    
    if(System_State_Struc_t.u8MainState == SYSTEM_MODE_SETTING)
    {
        if(SW_Struct_t.u16SwMSResetDelay < SW_MS_RESET_DELAY)
        {
            SW_Struct_t.u16SwMSResetDelay++;
        }
        else
        {
            SW_Struct_t.bSwMSReset = true;
        }
    }
    else
    {
        SW_Struct_t.u16SwMSResetDelay = 0;
        SW_Struct_t.bSwMSReset = false;
    }
    
    if(System_State_Struc_t.u8MainState == SYSTEM_POWER_ON)
    {
        if(System_State_Struc_t.u32PowerOnTimeOutDelay < POWER_OFF_DELAY)
        {
            System_State_Struc_t.u32PowerOnTimeOutDelay++;
        }
        else
        {
            System_State_Struc_t.bPowerOnTimeOut = true;
        }
    }
    else
    {
        System_State_Struc_t.u32PowerOnTimeOutDelay = 0;
        System_State_Struc_t.bPowerOnTimeOut = false;
    }
#endif
    
#if 0
    /* Motor Control Function */
    
    if(System_State_Struc_t.bPWM_ON == true)
    {
        switch (u16PwmState)
        {
            case PWM_RAMP_ON:
                TCA0.SINGLE.CTRLB = 0x2B;
                u16PwmDutyTemp = 0;
                u16PwmDutyStep = u16PwmDutyTarget >> 2;
                //u16PwmDutyStep = u16PwmDutyTarget / 3;
                u16PwmState = PWM_RAMP_ON_DELAY;
                
                u8Cnt = 0;
                break;
            case PWM_RAMP_ON_DELAY:
#if 0
                if(u16PwmDutyTemp > u16PwmDutyTarget)
                {
                    u16PwmDutyTemp = u16PwmDutyTarget;
                    u16PwmState = PWM_ON;
                }
                else
                {
                    u16PwmDutyTemp = u16PwmDutyTemp + u16PwmDutyStep;
                }
#else
                if(u8Cnt < SS_CNT)
                {
                    u8Cnt++;
                    if(u16PwmDutyTemp < u16PwmDutyTarget)
                    {
                        u16PwmDutyTemp = u16PwmDutyTemp + u16PwmDutyStep;
                    }
                    else
                    {
                        u16PwmDutyTemp = u16PwmDutyTarget;
                    }
                }
                else
                {
                    u16PwmDutyTemp = u16PwmDutyTarget;
                    u16PwmState = PWM_ON;
                }
#endif                
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
                u16PwmDutyStep = u16PwmDutyTarget >> 2;
                //u16PwmDutyStep = u16PwmDutyTarget / 3;
                u16PwmState = PWM_RAMP_OFF_DELAY;
                
                u8Cnt = 0;
                break;
            case PWM_RAMP_OFF_DELAY:
#if 0                
                if(u16PwmDutyTemp > u16PwmDutyStep)
                {
                    u16PwmDutyTemp = u16PwmDutyTemp - u16PwmDutyStep;
                }
                else
                {
                    u16PwmDutyTemp = 0;
                    u16PwmState = PWM_OFF;
                }
#else
                if(u8Cnt < (SS_CNT-1))
                {
                    u8Cnt++;
                    if(u16PwmDutyTemp > u16PwmDutyStep)
                    {
                        u16PwmDutyTemp = u16PwmDutyTemp - u16PwmDutyStep;
                    }
                    else
                    {
//                        PORTB_OUTSET = 0x20;                        
//                        if(u16PwmDutyTemp < PWM_DUTY_MIN)
//                        {
//                            u16PwmDutyTemp = PWM_DUTY_MIN;
//                        }
                    }
                    
                }
                else
                {
                    //u16PwmDutyTemp = 1;
                    u16PwmDutyTemp = PWM_DUTY_MIN;
                    u16PwmState = PWM_OFF;                   
                }
#endif
                TCA0.SINGLE.CMP1 = u16PwmDutyTemp;
                break;
            case PWM_OFF:
                PORTB_OUTSET = 0x20;
                TCA0.SINGLE.CMP1 = 0x00;        // set PWM duty = 0;
                TCA0.SINGLE.CTRLB = 0x00;
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
        
        TCA0.SINGLE.CTRLB = 0x00;
    }
#endif

#if 1
    // Battery Capacity Detection
    if(System_State_Struc_t.bAdcStart == true)
    {
        if(System_State_Struc_t.u16Battery < BATTERY_LOW_VOLTAGE)
        {
            if(System_State_Struc_t.u8BatteryLowDelay < BATTERY_LOW_DELAY)
            {
                System_State_Struc_t.u8BatteryLowDelay++;
            }
            else
            {
                System_State_Struc_t.bBatteryLow = true;
            }
            if(System_State_Struc_t.bBatteryLow == true)
            {
                if(System_State_Struc_t.u16Battery < BATTERY_ULTRA_LOW_VOLTAGE)
                {
                    System_State_Struc_t.bBatteryUltraLow = true;
                }
            }
        }
        else
        {
            System_State_Struc_t.u8BatteryLowDelay = 0;
        }
        ADC0_StartConversion(ADC_MUXPOS_AIN5_gc);
        System_State_Struc_t.bAdcStart = false;
    }
#endif
    
#if 1
    if(System_State_Struc_t.u8MainState == SYSTEM_POWER_OFF)
    {
        if(System_State_Struc_t.u8SleepCnt < SYSTEM_SLEEP_CNT)
        {
            System_State_Struc_t.u8SleepCnt++;
        }
        else
        {
            System_State_Struc_t.bSleep = true;
        }
    }
    else
    {
        System_State_Struc_t.u8SleepCnt = 0;
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
//    PORTB_OUTCLR = 0x20;
}

/**
 * \brief Initialize TCB interface
 */
int8_t TCB0_Initialize()
{
    //Compare or Capture
    TCB0.CCMP = 0x9C3;

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

    //RUNSTDBY disabled; SYNCUPD enabled; CLKSEL CLKTCA; ENABLE enabled; 
    TCB0.CTRLA = 0x15;

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
