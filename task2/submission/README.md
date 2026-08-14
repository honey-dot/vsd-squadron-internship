# Implementation in Task 2

- Flashing firmware onto the VSDSquadron Mini Board and making it run (board bring-up)
- Communicating through UART and monitoring it
- Identifying a real physical GPIO pin and its firmware equivalent
- Controlling that GPIO through a clean API

## Details About the Board Relevant to the Task

- **USB connector:** Type-C 2.0
- **USART RX (receiving):** PD6
- **USART TX (transmitting):** PD5
- **Built-in LED Pin:** PD6
- **Onboard RISC-V microcontroller:** CH32V003F4U6

## Main Initialization Requirements

- Install PlatformIO in VS Code
- Install CH32V platform
- Install VSDSquadron Mini board support repository

### Platform Configuration

- **Platform:** VSDSquadron
- **Hardware:** CH32V003F4U6
- **Framework:** `noneos-sdk`
- **Toolchain:** `toolchain-riscv`

### `noneos-sdk`

The `noneos-sdk` framework means the firmware runs directly on the microcontroller.

### Board Serial Interface

`/dev/cu.usbmodem4DAF8F06D2702`

---

# Blink Test

In `main.c`, `GPIO_WriteBit()` was directly used, which was useful for testing.

However, this was not suitable for the final programming structure as it directly exposed the hardware-specific implementation.

The **Build** and **Upload** options were used to test the LED blinking every second.

This verified that:

* The firmware compiled and flashed successfully
* The CPU booted
* The clock worked sufficiently
* The GPIO peripheral worked

## Build Error

The following build error was encountered:

```text
'NVIC_PriorityGroup_2' undeclared
did you mean 'NVIC_PriorityGroup_1'?
```

The value was changed from:

```c
NVIC_PriorityGroup_2
```

to:

```c
NVIC_PriorityGroup_1
```

---

# UART

* **Baud Rate:** 115200 baud
* **UART:** Used as a debugging interface

The firmware produced continuously increasing counter values in the serial monitor.

---

# GPIO Abstraction

Instead of keeping the GPIO hardware-specific operations directly inside `main.c`, a separate GPIO API was created.

## Files Created

* `gpio.h`
* `gpio.c`

### `gpio.h`

Declares the GPIO API functions:

* `gpio_init()`
* `gpio_set()`
* `gpio_clear()`
* `gpio_toggle()`

### `gpio.c`

Contains the actual implementation of the GPIO API.

### `gpio_init()`

* Enables the clock for GPIOD.
* Configures the selected pin as:

  * `GPIO_Mode_Out_PP`
  * `GPIO_Speed_50MHz`
* The configured GPIO is GPIOD.

### `gpio_set()`

Uses `GPIO_SetBits()` to drive the selected GPIO **HIGH**.

### `gpio_clear()`

Uses `GPIO_ResetBits()` to drive the selected GPIO **LOW**.

### `gpio_toggle()`

* Reads the current output state using `GPIO_ReadOutputDataBit()`.
* The state is inverted and written back using `GPIO_WriteBit()`.

This creates an abstraction layer between the application and the hardware-specific GPIO implementation.

---

# GPIO Pin Mapping

* **Physical board pin:** PD6
* **Firmware equivalent:** `GPIO_Pin_6`
* **GPIO peripheral:** GPIOD

The onboard LED is connected to PD6, so `GPIO_Pin_6` was used in the firmware to control the LED.

---

# Main Application

### `main.c` Includes

```c
#include <ch32v00x.h>
#include <debug.h>
#include "gpio.h"
```

The application defines:

```c
#define LED_PIN GPIO_Pin_6
```

This keeps the application code independent of the lower-level GPIO implementation.

## Initialization

During initialization:

```c
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
SystemCoreClockUpdate();
Delay_Init();
```

These initialize the required system configuration and delay functionality.

UART is then initialized at:

```c
USART_Printf_Init(115200);
```

Startup information is printed using `printf()`:

```text
VSDSquadron Mini
Firmware Version: Task2-v1.0
GPIO Test: PD6 On-board LED
```

The LED is initialized using:

```c
gpio_init(LED_PIN);
```

---

# LED and UART Test Loop

The firmware then enters an infinite `while(1)` loop.

A counter is printed continuously:

```c
printf("Counter: %lu\r\n", counter++);
```

The LED state is changed using the GPIO abstraction:

```c
if (ledState)
    gpio_set(LED_PIN);
else
    gpio_clear(LED_PIN);
```

The state is then inverted:

```c
ledState ^= 1;
```

Finally:

```c
Delay_Ms(1000);
```

creates a one-second delay before the next iteration.

Therefore, the firmware simultaneously demonstrates:

* UART communication
* GPIO control
* Periodic firmware execution

---

# Why GPIO Abstraction Was Used

The initial blink test directly used `GPIO_WriteBit()` inside `main.c`.

This was useful for quickly verifying that the LED and GPIO were working, but it directly exposed hardware-specific implementation to the application.

For the final implementation, the GPIO operations were moved into `gpio.c` and exposed through `gpio.h`.

### Initial Structure

```text
main.c
    ↓
GPIO_WriteBit()
    ↓
Hardware
```

### Final Structure

```text
main.c
    ↓
gpio_init()
gpio_set()
gpio_clear()
gpio_toggle()
    ↓
gpio.c
    ↓
CH32V GPIO SDK
    ↓
GPIOD / PD6
    ↓
On-board LED
```

This separates application logic from hardware-specific GPIO implementation and makes the GPIO functions reusable.

---

# Build Process

The project was built using the PlatformIO **Build** option.

PlatformIO compiled the application and the required framework components using the RISC-V toolchain.

Successful build generated:

```text
firmware.elf
firmware.bin
```

---

# Upload Process

After building successfully, the firmware was uploaded to the VSDSquadron Mini using the onboard programming interface.

---

# UART Monitoring

The firmware produced continuously increasing counter values in the serial monitor.

This verified that the firmware was executing continuously on the microcontroller and that UART communication with the computer was working.

---

# Hardware Verification

The onboard LED connected to PD6 blinked with approximately one-second intervals.

At the same time, the UART monitor displayed the increasing counter.

Therefore, both the physical GPIO output and UART communication were verified on the actual VSDSquadron Mini board.

