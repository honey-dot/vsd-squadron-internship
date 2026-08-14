#include "gpio.h"

void gpio_init(uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void gpio_set(uint16_t pin)
{
    GPIO_SetBits(GPIOD, pin);
}

void gpio_clear(uint16_t pin)
{
    GPIO_ResetBits(GPIOD, pin);
}

void gpio_toggle(uint16_t pin)
{
    GPIO_WriteBit(
        GPIOD,
        pin,
        (BitAction)(!GPIO_ReadOutputDataBit(GPIOD, pin))
    );
}