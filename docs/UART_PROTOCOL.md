# USART2 motion protocol

The firmware listens on USART2 at 115200 baud. Commands are ASCII strings
terminated with `$`.

Base move format:

- `e2e4` -> move from origin square to destination square

Optional suffix flags:

- `x` -> capture
- `&` -> king-side castling
- `@` -> queen-side castling
- `-` -> reverse / undo execution for the previously encoded move

Session control:

- A single space (`0x20`) starts a new game session.
- The firmware emits `*` when a motion sequence finishes.
