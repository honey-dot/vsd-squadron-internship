#include <ch32v00x.h>
#include <debug.h>
#include "gpio.h"

#define LED_PIN GPIO_Pin_6

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void Delay_Init(void);
void Delay_Ms(uint32_t n);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    USART_Printf_Init(115200);
    Delay_Ms(100);

    printf("VSDSquadron Mini\r\n");
    printf("Firmware Version: Task2-v1.0\r\n");
    printf("GPIO Test: PD6 On-board LED\r\n");

    gpio_init(LED_PIN);

    uint8_t ledState = 0;
    uint32_t counter = 1;

    while (1)
    {
        printf("Counter: %lu\r\n", counter++);

        if (ledState)
            gpio_set(LED_PIN);
        else
            gpio_clear(LED_PIN);

        ledState ^= 1;

        Delay_Ms(1000);
    }
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}