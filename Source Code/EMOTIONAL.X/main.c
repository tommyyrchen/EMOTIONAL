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

#include "mcc_generated_files/mcc.h"

int16_t i16Temp = 0;

/*
    Main application
*/
int main(void)
{
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();

    LED_Struct_t.u8LedState = LED_OFF;
    SW_Struct_t.bSWPower = false;
    System_State_Struc_t.u8MainState = SYSTEM_INIT;
    System_State_Struc_t.u8SubState = SYSTEM_SUB_INIT;
    System_State_Struc_t.u8SubMode = 4;
    System_State_Struc_t.bBatteryLow = false;
    System_State_Struc_t.bBatteryUltraLow = false;
    System_State_Struc_t.bPWM_ON = false;
    
    /* Replace with your application code */
    while (1){
        switch(System_State_Struc_t.u8MainState)
        {
            case SYSTEM_INIT:
                LED_Struct_t.u8LedState = LED_ORANGE_BLINK_2HZ_1_TIMES;
                if(LED_Struct_t.u8Times >= LED_BLINK_2HZ_1_TIMES)
                {
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_OFF;
                }
                break;
            case SYSTEM_POWER_OFF:
                LED_Struct_t.u8LedState = LED_OFF;
                System_State_Struc_t.bPWM_ON = false;
                ADC0.CTRLA = 0x00;  // ADC disabled
                
                if(SW_Struct_t.bSwMS == true)
                {
                    SW_Struct_t.bSwMS = false;
                    System_State_Struc_t.u8MainState = SYSTEM_MODE_SETTING;
                    ADC0.CTRLA = 0x01;  // ADC enabled
                    ADC0_StartConversion(ADC_MUXPOS_AIN5_gc);      
                }
                else if(SW_Struct_t.bSWPower == true)
                {
                    SW_Struct_t.bSWPower = false;
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_ON;
                    System_State_Struc_t.u8SubState = SYSTEM_SUB_INIT;
                    ADC0.CTRLA = 0x01;  // ADC enabled
                    ADC0_StartConversion(ADC_MUXPOS_AIN5_gc);
                }
                break;
            case SYSTEM_POWER_ON:                
                System_State_Struc_t.bPWM_ON = true;
                switch (System_State_Struc_t.u8SubState)
                {
                    case SYSTEM_SUB_INIT:
                        if(System_State_Struc_t.bBatteryLow == true)
                        {
                            LED_Struct_t.u8LedState = LED_RED_BLINK_2HZ;
                        }
                        else
                        {
                            LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_5_TIMES;
                        }
                        //u16PwmMode = PWM_MODE_DEFAULT + 4;
                        //u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        i16Temp = u16PwmMode;
                        u16PwmDutyTarget = (int16_t)PWM_DUTY_DEFAULT + (int16_t)(i16Temp * 8);
                        break;
                    case SYSTEM_SUB_MODE_1:
                        if(System_State_Struc_t.bBatteryLow == true)
                        {
                            LED_Struct_t.u8LedState = LED_RED_BLINK_2HZ;
                        }
                        else
                        {
                            LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_1_TIMES;
                        }
                        //u16PwmMode = PWM_MODE_DEFAULT;
                        //u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        i16Temp = (int16_t)((int16_t)u16PwmMode - 3);
                        u16PwmDutyTarget = (int16_t)PWM_DUTY_DEFAULT + (int16_t)(i16Temp * 8);
                        break;
                    case SYSTEM_SUB_MODE_2:
                        if(System_State_Struc_t.bBatteryLow == true)
                        {
                            LED_Struct_t.u8LedState = LED_RED_BLINK_2HZ;
                        }
                        else
                        {                        
                            LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_2_TIMES;
                        }
//                        u16PwmMode = PWM_MODE_DEFAULT + 1;
//                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        i16Temp = (int16_t)((int16_t)u16PwmMode - 2);
                        u16PwmDutyTarget = (int16_t)PWM_DUTY_DEFAULT + (int16_t)(i16Temp * 8);
                        break;
                    case SYSTEM_SUB_MODE_3:
                        if(System_State_Struc_t.bBatteryLow == true)
                        {
                            LED_Struct_t.u8LedState = LED_RED_BLINK_2HZ;
                        }
                        else
                        {                        
                            LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_3_TIMES;
                        }
//                        u16PwmMode = PWM_MODE_DEFAULT + 2;
//                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        i16Temp = (int16_t)((int16_t)u16PwmMode - 1);
                        u16PwmDutyTarget = (int16_t)PWM_DUTY_DEFAULT + (int16_t)(i16Temp * 8);
                        break;
                    case SYSTEM_SUB_MODE_4:
                        if(System_State_Struc_t.bBatteryLow == true)
                        {
                            LED_Struct_t.u8LedState = LED_RED_BLINK_2HZ;
                        }
                        else
                        {                        
                            LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_4_TIMES;
                        }
//                        u16PwmMode = PWM_MODE_DEFAULT + 3;
//                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        i16Temp = u16PwmMode;
                        u16PwmDutyTarget = (int16_t)PWM_DUTY_DEFAULT + (int16_t)(i16Temp * 8);
                        break;
                    default:
                        break;
                }
                
                
                if(SW_Struct_t.bSwModeReset == true)
                {
                    SW_Struct_t.bSwModeReset = false;
                    SW_Struct_t.bSWMode = false;
                    System_State_Struc_t.u8SubMode = 4;
                    System_State_Struc_t.u8SubState = SYSTEM_SUB_MODE_4;                   
                }
                else
                {
                    if(SW_Struct_t.bSWMode == true)
                    {
                        SW_Struct_t.bSWMode = false;
                        if(System_State_Struc_t.u8SubMode < 4)
                        {
                            System_State_Struc_t.u8SubMode++;
                        }
                        else
                        {
                            System_State_Struc_t.u8SubMode = 1;
                        }
                        System_State_Struc_t.u8SubState = System_State_Struc_t.u8SubMode;
                    }
                }
                
                //if(SW_Struct_t.bSWPower == false || System_State_Struc_t.bBatteryUltraLow == true)
                if(SW_Struct_t.bSWPower == true)
                {
                    SW_Struct_t.bSWPower = false;
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_OFF;
                }
                else if(System_State_Struc_t.bBatteryUltraLow == true)
                {
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_OFF;                    
                }
                
                break;
            case SYSTEM_MODE_SETTING:
                System_State_Struc_t.bPWM_ON = true;
                LED_Struct_t.u8LedState = LED_ORANGE_SOLID;
                
                if(SW_Struct_t.bSWPower == true)
                {
                    SW_Struct_t.bSWPower = false;
                    if(u16PwmMode < 20)
                    {
                        u16PwmMode++;
                    }
                }
                else if(SW_Struct_t.bSWMode == true)
                {
                    SW_Struct_t.bSWMode = false;
                    if(u16PwmMode > 0)
                    {
                        u16PwmMode--;
                    }
                }
                else if(SW_Struct_t.bSwMS == true)
                {
                    SW_Struct_t.bSwMS = false;
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_OFF;
                }
                else if(SW_Struct_t.bSwMSReset == true)
                {
                    SW_Struct_t.bSwMSReset = false;
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_OFF;
                }
                
                u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode * 8);
                
                break;
            case SYSTEM_POWER_LOW:
                break;
            default:
                break;
        }
    }
}
/**
    End of File
*/