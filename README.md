# ucprintf
## Minimal printf for microcontrollers

Contains a minimal printf implementation.

Format string: `%(+)(width).(precision)(size)[specifier]` 

Supported specifiers: `s, d, u, x, X, f, o`

Supported size modifiers: `hh, h, l, ll`

## Usage
Add `ucprintf.c` and `.h` to your project. The application must implement `void _putchar(char)` function, which is used to send characters to the output device. Additionnaly, `void _flush(void)` can be implemented to trigger buffered transfers.

## TODO
 - [ ] Support for scientific float notation (`e, E`)
 - [ ] Refactoring, code cleanup & optimization (In progress)
 - [ ] scanf() counterpart
