# Consolidation changelog

## Structural changes

- Converted the repository into a standalone STM32CubeIDE project.
- Localized the required peripheral drivers under `Drivers/`.
- Added the missing STM32F4 device headers under `Core/Device/.../Include`.
- Added `Core/Src/system_stm32f4xx.c` to provide `SystemInit()`.
- Updated CubeIDE metadata so the project no longer depends on an external academic workspace.

## Source hygiene changes

- Removed the leading whitespace from the original `GPIOxDriver.c` filename.
- Replaced non-ASCII identifier names in the application source to improve portability.
- Cleaned the application header banner and refreshed project documentation.
- Added a host-side Python CLI example and a short UART protocol note.

## Intentionally preserved

- Step-counting logic
- Homing behavior
- Endstop callbacks
- Capture / castling handling
- USART2 message protocol used by the firmware
