# Skyd Drop System Firmware

Application firmware for STM32F103C8T6 based UAV drop system PCB.

Runs at `0x08003000`, loaded by the custom UART bootloader.

Built with STM32CubeIDE / HAL.

---

## Overview

Controls a UAV payload drop mechanism including stopper and pusher servos, heater, temperature sensing, and switch monitoring. Communicates with the Skyd ground station configurator over UART using a custom MAVLink dialect (Skynet).

---

## Hardware

| Peripheral | Pin | Description |
|-----------|-----|-------------|
| USART1 TX | PB6 | Upstream UART to configurator (remapped) |
| USART1 RX | PB7 | Upstream UART from configurator (remapped) |
| USART2 | — | Downstream UART for daisy-chain |
| TIM1 CH1/CH3 | — | Servo PWM output |
| TIM1 CH2 | — | Heater driver PWM |
| TIM2 | — | DS18B20 one-wire timing |
| TIM4 | — | PWM input capture |
| DS18B20 | PB10 | Temperature sensor (one-wire) |
| LED | PB13 | Status LED |
| SWD | PA13/PA14 | Debug interface |

---

## Flash Memory Layout

```
0x08000000   Bootloader        12KB
0x08003000   Application       60KB   ← starts here
0x08012000   FW Valid Flag      1KB   ← 0xCAFECAFE written by linker
0x08012400   Runtime Flags      1KB   ← config, preserved across updates
0x08020000   Flash end         128KB
```

---

## Project Structure

```
firmware/
├── Core/
│   ├── Src/
│   │   ├── main.c               # Entry point, peripheral init
│   │   ├── app/
│   │   │   ├── drop_system.c    # Drop sequence orchestrator
│   │   │   ├── heater.c         # Heater control
│   │   │   ├── thermo.c         # DS18B20 temperature
│   │   │   └── comms.c          # MAVLink communication
│   │   └── drivers/
│   │       ├── ds18b20.c        # One-wire temperature driver
│   │       ├── servo.c          # PWM servo driver
│   │       └── flash_flags.c    # Flash config storage
│   └── Inc/
│       ├── flash_flags.h
│       └── fw_flag.h
├── fw_flag.c                    # Firmware valid flag — DO NOT REMOVE
└── STM32F103C8TX_FLASH.ld       # Linker script
```

---

## Architecture

Three-layer architecture — strict top-down dependency:

```
main.c  (orchestrator)
  │
  ├── App/          — business logic, state machines
  │     drop_system.c
  │     heater.c
  │     thermo.c
  │     comms.c
  │
  └── Drivers/      — hardware abstraction
        ds18b20.c
        servo.c
        flash_flags.c
```

Upper layers call lower layers. Lower layers never call up.

---

## Communication Protocol

USART1 at 115200 8N1, DMA receive.

Uses a custom MAVLink dialect (**Skynet**). The configurator sends debug commands and receives state packets.

### State Packet (device → configurator)

Sent continuously, contains:
- Stopper position
- Pusher position
- Switch states (fixator, counter, end, start)
- Min/max stopper config values
- Heater state
- Temperature (DS18B20)
- Flag mask
- Firmware version

### Debug Commands (configurator → device)

| Value | Command |
|-------|---------|
| 0 | STOPER_OPEN |
| 1 | STOPER_IDLE |
| 2 | STOPER_CLOSE |
| 3 | PUSHER_HOME |
| 4 | PUSHER_STOP |
| 5 | PUSHER_TO_FINISH |
| 6 | PUSHER_DROP |
| 7 | EMERGENCY_STOP |
| 8 | RESTART_SERVOS |
| 9 | START_HEATING |
| 10 | STOP_HEATING |
| 50 | JUMP_TO_BOOTLOADER |
| 51 | RESTART_CONTROLLER |

---

## Flash Config Storage

Runtime configuration (servo limits, thresholds, etc.) stored in flash at `0x08012400`.

Uses append-log pattern — no erase on every write. Page erased only when full (~64 writes).

```c
// Read config
const FlashFlags* cfg = read_flash_flags();

// Update one field
FlashFlags f = *cfg;
f.hw_version = 2;
write_flash_flags(&f);
```

Config page is **never erased by the OTA flasher** — settings survive firmware updates.

---

## Firmware Valid Flag

`fw_flag.c` places `0xCAFECAFE` at `0x08012000` via linker section `.fw_flag`.

```c
const uint32_t firmware_valid_flag
    __attribute__((section(".fw_flag")))
    __attribute__((used))
    = 0xCAFECAFE;
```

**Do not remove this file.** Without it the bootloader will refuse to boot the app and wait for a new firmware indefinitely.

---

## OTA Firmware Update

App triggers OTA update by writing magic value to RTC backup register and resetting:

```c
void request_bootloader(void) {
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    PWR->CR      |= PWR_CR_DBP;
    BKP->DR1      = 0xDEAD;
    NVIC_SystemReset();
}
```

Triggered by `JUMP_TO_BOOTLOADER` command from configurator.

---

## Linker Script

App must start at `0x08003000`:

```ld
MEMORY
{
  RAM     (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH   (rx)  : ORIGIN = 0x08003000, LENGTH = 60K
  FW_FLAG (rx)  : ORIGIN = 0x08012000, LENGTH = 1K
  FLAGS   (rx)  : ORIGIN = 0x08012400, LENGTH = 1K
}
```

---

## Vector Table

`system_stm32f1xx.c` must have:

```c
#define USER_VECT_TAB_ADDRESS
#define VECT_TAB_BASE_ADDRESS  FLASH_BASE
#define VECT_TAB_OFFSET        0x00003000U
```

And `main.c` must set it explicitly before `HAL_Init()`:

```c
int main(void) {
    __enable_irq();              // re-enable after bootloader jump
    SCB->VTOR = 0x08003000;     // set vector table
    HAL_Init();
    SystemClock_Config();
    // ...
}
```

---

## Building

Build normally in STM32CubeIDE.

Generate `.bin` file — add post-build command under:
```
Project → Properties → C/C++ Build → Settings → MCU Post build outputs
→ Enable: Convert to binary file (-O binary)
```

Output: `build/Release/SKYD.bin` — flash this file via configurator OTA or ST-Link.

---

## Flashing

**Via OTA** (normal workflow):
1. Open Skyd Configurator
2. Connect to device
3. Go to Firmware tab
4. Select `.bin` file
5. Click Flash

**Via ST-Link** (initial flash or recovery):
```bash
STM32_Programmer_CLI -c port=SWD -w SKYD.bin 0x08003000 -s
```

---

## Common Issues

**App hangs immediately after bootloader jump**
→ Missing `__enable_irq()` at start of `main()`. Bootloader disables interrupts before jumping.

**Interrupts not firing (HAL_Delay hangs, DMA silent)**
→ Wrong `VECT_TAB_OFFSET` or missing `SCB->VTOR` assignment in `main()`.

**Device reboots into bootloader instead of running app**
→ `fw_flag.c` missing from project, or `FW_FLAG` section missing from linker script.

**Every other UART byte dropped**
→ `MX_DMA_Init()` must be called before `MX_USART1_UART_Init()`.

**Temperature sensor stuck in busy loop**
→ TIM2 must be initialized and started before `thermo_init()` is called.
