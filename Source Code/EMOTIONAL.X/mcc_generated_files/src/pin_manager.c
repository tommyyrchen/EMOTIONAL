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

#include "../include/pin_manager.h"
static void (*PORTB_PB2_InterruptHandler)(void);
static void (*PORTC_SW_MODE_InterruptHandler)(void);
static void (*PORTA_PA5_InterruptHandler)(void);
static void (*PORTC_IO_PC0_InterruptHandler)(void);
static void (*PORTB_PB1_InterruptHandler)(void);
static void (*PORTC_SW_POPWER_InterruptHandler)(void);
static void (*PORTC_IO_PC1_InterruptHandler)(void);
static void (*PORTB_PB0_InterruptHandler)(void);

void PORT_Initialize(void);

void PIN_MANAGER_Initialize()
{
    PORT_Initialize();

    /* DIR Registers Initialization */
    PORTA.DIR = 0x00;
    PORTB.DIR = 0x02;
    PORTC.DIR = 0x03;

    /* OUT Registers Initialization */
    PORTA.OUT = 0x00;
    PORTB.OUT = 0x00;
    PORTC.OUT = 0x00;

    /* PINxCTRL registers Initialization */
    PORTA.PIN0CTRL = 0x00;
    PORTA.PIN1CTRL = 0x00;
    PORTA.PIN2CTRL = 0x00;
    PORTA.PIN3CTRL = 0x00;
    PORTA.PIN4CTRL = 0x00;
    PORTA.PIN5CTRL = 0x00;
    PORTA.PIN6CTRL = 0x00;
    PORTA.PIN7CTRL = 0x00;
    PORTB.PIN0CTRL = 0x00;
    PORTB.PIN1CTRL = 0x00;
    PORTB.PIN2CTRL = 0x00;
    PORTB.PIN3CTRL = 0x00;
    PORTB.PIN4CTRL = 0x00;
    PORTB.PIN5CTRL = 0x00;
    PORTB.PIN6CTRL = 0x00;
    PORTB.PIN7CTRL = 0x00;
    PORTC.PIN0CTRL = 0x00;
    PORTC.PIN1CTRL = 0x00;
    PORTC.PIN2CTRL = 0x00;
    PORTC.PIN3CTRL = 0x00;
    PORTC.PIN4CTRL = 0x00;
    PORTC.PIN5CTRL = 0x00;
    PORTC.PIN6CTRL = 0x00;
    PORTC.PIN7CTRL = 0x00;

    /* PORTMUX Initialization */
    PORTMUX.CTRLA = 0x00;
    PORTMUX.CTRLB = 0x00;
    PORTMUX.CTRLC = 0x00;
    PORTMUX.CTRLD = 0x00;

    // register default ISC callback functions at runtime; use these methods to register a custom function
    PORTB_PB2_SetInterruptHandler(PORTB_PB2_DefaultInterruptHandler);
    PORTC_SW_MODE_SetInterruptHandler(PORTC_SW_MODE_DefaultInterruptHandler);
    PORTA_PA5_SetInterruptHandler(PORTA_PA5_DefaultInterruptHandler);
    PORTC_IO_PC0_SetInterruptHandler(PORTC_IO_PC0_DefaultInterruptHandler);
    PORTB_PB1_SetInterruptHandler(PORTB_PB1_DefaultInterruptHandler);
    PORTC_SW_POPWER_SetInterruptHandler(PORTC_SW_POPWER_DefaultInterruptHandler);
    PORTC_IO_PC1_SetInterruptHandler(PORTC_IO_PC1_DefaultInterruptHandler);
    PORTB_PB0_SetInterruptHandler(PORTB_PB0_DefaultInterruptHandler);
}

void PORT_Initialize(void)
{
    /* On AVR devices all peripherals are enable from power on reset, this
     * disables all peripherals to save power. Driver shall enable
     * peripheral if used */

    /* Set all pins to low power mode */
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTC + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
}

/**
  Allows selecting an interrupt handler for PORTB_PB2 at application runtime
*/
void PORTB_PB2_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_PB2_InterruptHandler = interruptHandler;
}

void PORTB_PB2_DefaultInterruptHandler(void)
{
    // add your PORTB_PB2 interrupt custom code
    // or set custom function using PORTB_PB2_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_SW_MODE at application runtime
*/
void PORTC_SW_MODE_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_SW_MODE_InterruptHandler = interruptHandler;
}

void PORTC_SW_MODE_DefaultInterruptHandler(void)
{
    // add your PORTC_SW_MODE interrupt custom code
    // or set custom function using PORTC_SW_MODE_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA5 at application runtime
*/
void PORTA_PA5_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA5_InterruptHandler = interruptHandler;
}

void PORTA_PA5_DefaultInterruptHandler(void)
{
    // add your PORTA_PA5 interrupt custom code
    // or set custom function using PORTA_PA5_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_IO_PC0 at application runtime
*/
void PORTC_IO_PC0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_IO_PC0_InterruptHandler = interruptHandler;
}

void PORTC_IO_PC0_DefaultInterruptHandler(void)
{
    // add your PORTC_IO_PC0 interrupt custom code
    // or set custom function using PORTC_IO_PC0_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTB_PB1 at application runtime
*/
void PORTB_PB1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_PB1_InterruptHandler = interruptHandler;
}

void PORTB_PB1_DefaultInterruptHandler(void)
{
    // add your PORTB_PB1 interrupt custom code
    // or set custom function using PORTB_PB1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_SW_POPWER at application runtime
*/
void PORTC_SW_POPWER_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_SW_POPWER_InterruptHandler = interruptHandler;
}

void PORTC_SW_POPWER_DefaultInterruptHandler(void)
{
    // add your PORTC_SW_POPWER interrupt custom code
    // or set custom function using PORTC_SW_POPWER_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_IO_PC1 at application runtime
*/
void PORTC_IO_PC1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_IO_PC1_InterruptHandler = interruptHandler;
}

void PORTC_IO_PC1_DefaultInterruptHandler(void)
{
    // add your PORTC_IO_PC1 interrupt custom code
    // or set custom function using PORTC_IO_PC1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTB_PB0 at application runtime
*/
void PORTB_PB0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTB_PB0_InterruptHandler = interruptHandler;
}

void PORTB_PB0_DefaultInterruptHandler(void)
{
    // add your PORTB_PB0 interrupt custom code
    // or set custom function using PORTB_PB0_SetInterruptHandler()
}
