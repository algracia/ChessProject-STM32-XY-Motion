# ChessProject - Autonomous Chessboard (Professional Consolidated Build)

Standalone STM32F411 firmware project for an autonomous chessboard with XY
motion, endstop-based homing, servo magnet control, and a lightweight
USART2 protocol for host-side orchestration.

This package consolidates the firmware repository with the minimum STM32/CMSIS
and peripheral-driver dependencies required to keep the chessboard project
self-contained inside a single importable CubeIDE project.

## What was consolidated

- Added the missing STM32F4 device headers required by `stm32f4xx.h`
- Added only the peripheral drivers used by the chessboard firmware:
  GPIO, basic timers, EXTI, USART, SysTick, and PWM
- Added `system_stm32f4xx.c` so the startup file has a concrete `SystemInit`
  implementation
- Removed the dependency on an external `PeripheralDrivers` sibling project
- Preserved the existing motion logic and UART protocol used by the firmware

## Project layout

- `App/Src/AutonomousChess_Main.c` - application entry point and motion logic
- `Drivers/Inc`, `Drivers/Src` - minimal local driver set used by the firmware
- `Core/Include` - CMSIS core headers
- `Core/Device/ST/STM32F4xx/Include` - STM32F411 device headers
- `Core/Src/system_stm32f4xx.c` - system initialization source
- `tools/python/uci_serial_cli.py` - optional host-side CLI bridge for Stockfish + serial
- `docs/UART_PROTOCOL.md` - serial command summary

## Target

- MCU: STM32F411RETx
- Board profile in the existing CubeIDE metadata: NUCLEO-F411RE

## Importing in STM32CubeIDE

1. Open **File -> Import -> Existing Projects into Workspace**.
2. Select this folder as the project root.
3. Build the `Debug` configuration.

## Notes

- The firmware motion logic was intentionally preserved to avoid changing
  behavior that already works on the physical prototype.
- The included Python tool is optional and is not required to compile the
  embedded firmware.
