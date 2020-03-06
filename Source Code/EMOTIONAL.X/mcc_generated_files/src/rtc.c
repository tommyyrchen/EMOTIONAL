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

#include "../include/rtc.h"
#include "../mcc.h"
#include "../include/tcb0.h"

/**
 * \brief Initialize rtc interface
 *
 * \return Initialization status.
 */

void (*RTC_OVF_isr_cb)(void) = NULL;
void (*RTC_CMP_isr_cb)(void) = NULL;
void (*RTC_PIT_isr_cb)(void) = NULL;

/**
 * \brief Initialize RTC interface
 */
int8_t RTC_Initialize()
{
        while (RTC.STATUS > 0) { /* Wait for all register to be synchronized */
    }
    //Compare 
    RTC.CMP = 0x00;

    //Count
    RTC.CNT = 0x00;

    //Period
    RTC.PER = 0xFFFF;

    //Clock selection
    RTC.CLKSEL = 0x00;

    //DBGRUN disabled; 
    RTC.DBGCTRL = 0x00;

    //CMP disabled; OVF disabled; 
    RTC.INTCTRL = 0x00;

    //PERIOD CYC32768; PITEN enabled; 
    RTC.PITCTRLA = 0x51;    //0x71;

    //DBGRUN disabled; 
    RTC.PITDBGCTRL = 0x00;

    RTC.PITINTCTRL = 0x01;
    
    //RUNSTDBY disabled; PRESCALER DIV1; RTCEN enabled; 
    RTC.CTRLA = 0x01;

    return 0;
}

void RTC_SetOVFIsrCallback(RTC_cb_t cb)
{
    RTC_OVF_isr_cb = cb;
}

void RTC_SetCMPIsrCallback(RTC_cb_t cb)
{
    RTC_CMP_isr_cb = cb;
}

ISR(RTC_CNT_vect)
{
    //PORTB_OUTSET = 0x20;
    if (RTC.INTFLAGS & RTC_OVF_bm )
    {
        if (RTC_OVF_isr_cb != NULL) 
        {
            (*RTC_OVF_isr_cb)();
        } 
    }  
    
    if (RTC.INTFLAGS & RTC_CMP_bm )
    {
        if (RTC_CMP_isr_cb != NULL) 
        {
            (*RTC_CMP_isr_cb)();
        } 
    }   
    RTC.INTFLAGS = (RTC_OVF_bm | RTC_CMP_bm);
    //PORTB_OUTCLR = 0x20;
}

ISR(RTC_PIT_vect)
{
    
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
                u16PwmDutyStep = u16PwmDutyTarget >> 1;
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
#if 1
                if(i16Cnt > T_HOLD_BRIGHT)
                {
                    u16PwmState = PWM_RAMP_OFF;
                    i16Cnt = 0;
                }
                else
                {
                    i16Cnt++;
                }
#else
                System_State_Struc_t.bPwmOnDelay = true;
                if(System_State_Struc_t.bPwmOnDelayTimeOut == true)
                {
                    System_State_Struc_t.bPwmOnDelay = false;
                    System_State_Struc_t.bPwmOnDelayTimeOut = false;
                    u16PwmState = PWM_RAMP_OFF;
                }
#endif
                break;
            case PWM_RAMP_OFF:
                u16PwmDutyTemp = u16PwmDutyTarget;
                u16PwmDutyStep = u16PwmDutyTarget >> 1;
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
#if 1
                if(i16Cnt > T_HOLD_DULL)
                {
                   u16PwmState =  PWM_RAMP_ON;
                   i16Cnt = 0;
                }
                else
                {
                    i16Cnt++;
                }
#else
                System_State_Struc_t.bPwmOffDelay = true;
                if(System_State_Struc_t.bPwmOnDelayTimeOut == true)
                {
                    System_State_Struc_t.bPwmOffDelay = false;
                    System_State_Struc_t.bPwmOnDelayTimeOut = false;
                    u16PwmState =  PWM_RAMP_ON;
                }
#endif
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
//   if (RTC_PIT_isr_cb != NULL) 
//   {
//    (*RTC_PIT_isr_cb)();
//   } 
   //RTC.INTFLAGS = RTC_PI_bm;
   RTC.PITINTFLAGS = RTC_PI_bm;
   //PORTB_OUTCLR = 0x20;
}

inline void RTC_WriteCounter(uint16_t timerVal)
{
    while (RTC.STATUS & RTC_CNTBUSY_bm);
    RTC.CNT = timerVal;
}

inline uint16_t RTC_ReadCounter(void)
{
    return RTC.CNT;
}

inline void RTC_WritePeroid(uint16_t timerVal)
{
    while (RTC.STATUS & RTC_PERBUSY_bm);
    RTC.PER = timerVal;
}

inline uint16_t RTC_ReadPeriod(void)
{
    return RTC.PER;
}

inline void RTC_EnableCMPInterrupt(void)
{
    RTC.INTCTRL |= RTC_CMP_bm; 
}

inline void RTC_DisableCMPInterrupt(void)
{
    RTC.INTCTRL &= ~RTC_CMP_bm; 
}

inline void RTC_EnableOVFInterrupt(void)
{
    RTC.INTCTRL |= RTC_OVF_bm; 
}

inline void RTC_DisableOVFInterrupt(void)
{
    RTC.INTCTRL &= ~RTC_OVF_bm; 
}

inline void RTC_EnablePITInterrupt(void)
{
    RTC.INTCTRL |= RTC_PI_bm;  
}

inline void RTC_DisablePITInterrupt(void)
{
    RTC.INTCTRL &= ~RTC_PI_bm; 
}

inline void RTC_ClearOVFInterruptFlag(void)
{
    RTC.INTFLAGS &= ~RTC_OVF_bm;
}

inline bool RTC_IsOVFInterruptEnabled(void)
{
    return ((RTC.INTCTRL & RTC_OVF_bm) > 0);
}