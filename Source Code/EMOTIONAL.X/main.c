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
                if(SW_Struct_t.bSWPower == true)
                {
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_ON;
                    System_State_Struc_t.u8SubState = SYSTEM_SUB_INIT;
                }
                break;
            case SYSTEM_POWER_ON:
                
                switch (System_State_Struc_t.u8SubState)
                {
                    case SYSTEM_SUB_INIT:
                        LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_5_TIMES;                        
                        break;
                    case SYSTEM_SUB_MODE_1:
                        LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_1_TIMES;
                        u16PwmMode = PWM_MODE_DEFAULT;
                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        break;
                    case SYSTEM_SUB_MODE_2:
                        LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_2_TIMES;
                        u16PwmMode = PWM_MODE_DEFAULT + 1;
                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        break;
                    case SYSTEM_SUB_MODE_3:
                        LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_3_TIMES;
                        u16PwmMode = PWM_MODE_DEFAULT + 2;
                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        break;
                    case SYSTEM_SUB_MODE_4:
                        LED_Struct_t.u8LedState = LED_GREEN_BLINK_2HZ_4_TIMES;
                        u16PwmMode = PWM_MODE_DEFAULT + 3;
                        u16PwmDutyTarget = PWM_DUTY_DEFAULT + (u16PwmMode << 3);
                        break;
                    default:
                        break;
                }
                
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
                
                if(SW_Struct_t.bSWPower == false)
                {
                    System_State_Struc_t.u8MainState = SYSTEM_POWER_OFF;
                }
                break;
            case SYSTEM_MODE_SETTING:
                break;
            case SYSTEM_MODE_RESET:
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