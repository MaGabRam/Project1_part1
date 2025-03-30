#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.c"
#include <string.h>
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif
uint32_t FS=120000000*1.5; //120 MHz
void timer0A_handler(void);
uint8_t switch_state=0;
char  msg[]="h";
char data[100];
char c[100];
int aux=0;
uint32_t ui32Value=0;
uint32_t reloj;
volatile uint32_t width;
#define TIMEOUT 1000000 
int
main(void)
{
    
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, 0x03); // PN0 y PN1
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // PF0 y PF4
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, 0x03); // PJ0 (USR_SW1) y PJ1 (USR_SW2)

    GPIOPadConfigSet(GPIO_PORTJ_BASE, 0x03, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    
    // //TIMER
    // //enable the timer peripheral
    // SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // // Set timer
    // TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // // Set the count time for Timer
    // TimerLoadSet(TIMER0_BASE, TIMER_A, FS);
    // //Enable processor interrupts
    // IntMasterEnable();
    // // Enable Interrupt
    // IntEnable(35);//INT_TIMER0A_TM4C129
    // // Enable timer A interrupt
    // TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // // Enable the timer
    // TimerEnable(TIMER0_BASE, TIMER_A);
//   //UART
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

//     GPIOPinConfigure(GPIO_PA0_U0RX);
//     GPIOPinConfigure(GPIO_PA1_U0TX);
//     GPIOPinTypeUART(GPIO_PORTA_BASE, 0x03);

//     UARTStdioConfig(0, 9600, 120000000);
    //PWM
    // Enable the PWM0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);


    // Configure the PWM function for this pin.
    GPIOPinConfigure(GPIO_PF1_M0PWM1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

    // wait for the PWM0 module to be ready.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
    {
    }
    // Configure the PWM generator for count down mode with immediate updates to the parameters.
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20 microseconds.
    // For a 20 MHz clock, this translates to 400 clock ticks. Use this value to set the period.
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
    /*🔹 PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
→ Establece el período del PWM (ciclo completo) como 400 ticks.
→ Esto define el 100% de ciclo.

🔹 PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, width);
→ Cambia la anchura del pulso, o sea cuánto tiempo está en HIGH.
→ Por tanto, si width = 200, el duty cycle es del 50% (200 / 400). */

    // Set the pulse width of PWM1 for a 75% duty cycle (width = 300).
    width = 1;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, width);

    // Start the timers in generator 0.
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    // Enable the outputs.
    PWMOutputState(PWM0_BASE, (PWM_OUT_1_BIT), true);

    while(1)
    {   
        //UART  
        // uint8_t buttonState = GPIOPinRead(GPIO_PORTJ_BASE, 0x01);
        // // Changing the timer delay
        // if (buttonState == 0x00 ) 
        // {
        //     UARTprintf("motor1\n");
        // }
        // uint8_t buttonState2 = GPIOPinRead(GPIO_PORTJ_BASE, 0x02);
        // // Changing the timer delay
        // if (buttonState2 == 0x00 ) 
        // {
        //     UARTprintf("motor2\n");

        // }
        // while(UARTCharsAvail(UART0_BASE)){
        //     UARTgets(data,100);
        //     if(strcmp(data, "buzzer") == 0){
        //         aux=1;
        //        // SysCtlDelay(80000000);
        //         break;
        //     }
        //     else{
        //         aux=0;
        //         break;
        //     }
            
        // }
        // if(aux==1){
        //     GPIOPinWrite(GPIO_PORTF_BASE, 0x03, 0x03); 
        //     SysCtlDelay(80000000);
        //     GPIOPinWrite(GPIO_PORTF_BASE, 0x03, 0x00);
        //        }
        
        // SysCtlDelay(800000); // ~1 segundo (depende de clock)
        // Trigger the sample sequence OF ADC.
        // ADCProcessorTrigger(ADC0_BASE, 3);
        // // Wait until the sample sequence has completed.
        // while(!ADCIntStatus(ADC0_BASE, 3, false))
        // {
        // }
        // //Clear the ADC interruption
        // ADCIntClear(ADC0_BASE, 3);
        // // Read the value from the ADC.
        // ADCSequenceDataGet(ADC0_BASE, 3, &ui32Value);
        // //Operates with the ADC values
        // if (ui32Value > 2047)
        // {
        //     GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x03);
        //     GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x11);
        // }
        // else
        // {
        //     GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
        //     GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x00);
        // }
        
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, width);
        SysCtlDelay((120000000/3)*0.5);
        width+=4;
        
        if( width >= 400) {
        width = 1;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        PWMOutputState(PWM0_BASE, (PWM_OUT_1_BIT), false);
        SysCtlDelay(120000000);
        PWMOutputState(PWM0_BASE, (PWM_OUT_1_BIT), true);


        }
    }
}
void timer0A_handler(void)
{
    // switch_state++;
    // // Clear timer
    // TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // if (switch_state == 1)  
    // {
    //     GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
    //     GPIOPinWrite(GPIO_PORTF_BASE, 0x13, 0x00);
    // }
    // else if (switch_state == 2)  
    // {
    //     GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
    //     GPIOPinWrite(GPIO_PORTF_BASE, 0x13, 0x01);
    // }
    // else if (switch_state == 3)  
    // {
    //     GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
    //     GPIOPinWrite(GPIO_PORTF_BASE, 0x13, 0x11);
    // }
    // else if (switch_state == 4)  
    // {
    //     GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x01);
    //     GPIOPinWrite(GPIO_PORTF_BASE, 0x13, 0x11);
    //     switch_state=0;
    // }
}
