#include <stdio.h>

#include "ucprintf.h"

void _putchar(char c)
{
    putchar(c);
}

#define TEST_PRINTF(fmt, ...) \
    ucprintf("TEST: "fmt"\n", __VA_ARGS__); \
    printf("REF : "fmt"\n", __VA_ARGS__); \
    putchar('\n');

int main(void)
{
    TEST_PRINTF("%s %s", "Hello there!", "General Kenobi!")
    TEST_PRINTF("Some integers: %+.30d, %30d, %.5d", 42, -346650169, -69);
    TEST_PRINTF("Some hex integers: 0x%x, 0x%X", 0xdeadbeef, 0xbadface);
    TEST_PRINTF("Some negative hex integers: 0x%x, 0x%.12X", -1, -0xdead);
    TEST_PRINTF("Some octal integers: 0%o, 0%o", 8, -27);
    TEST_PRINTF("Some floats: %+.5f, %9.4f, %f", 3.14159f, -23.8f, -0.0f);

    return 0;
}
