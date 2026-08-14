#ifndef GPIO_H
#define GPIO_H

#include <ch32v00x.h>

void gpio_init(uint16_t pin);
void gpio_set(uint16_t pin);
void gpio_clear(uint16_t pin);
void gpio_toggle(uint16_t pin);

#endif