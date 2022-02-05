#include <stdio.h>
#include <limits.h>
#include <float.h>

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

    TEST_PRINTF("CHAR_MAX: %hhd CHAR_MIN: %hhd", (char)CHAR_MAX, (char)CHAR_MIN);
    TEST_PRINTF("SHRT_MAX: %hd SHRT_MIN: %hd", (short)SHRT_MAX, (short)SHRT_MIN);
    TEST_PRINTF("INT_MAX: %d INT_MIN: %d", INT_MAX, INT_MIN);
    TEST_PRINTF("LONG_MAX: %ld LONG_MIN: %ld", LONG_MAX, LONG_MIN);
    TEST_PRINTF("LLONG_MAX: %lld LLONG_MIN: %lld", LLONG_MAX, LLONG_MIN);

    TEST_PRINTF("UCHAR_MAX: %hhu", (unsigned char)UCHAR_MAX);
    TEST_PRINTF("USHRT_MAX: %hu", (unsigned short)USHRT_MAX);
    TEST_PRINTF("UINT_MAX: %u", UINT_MAX);
    TEST_PRINTF("ULONG_MAX: %lu", ULONG_MAX);
    TEST_PRINTF("ULLONG_MAX: %llu", ULLONG_MAX);

    TEST_PRINTF("Some integers: %+.30d, %30d, %.5d", 42, -346650169, -69);

    TEST_PRINTF("Some hex integers: 0x%x, 0x%X", 0xdeadbeef, 0xbadface);
    TEST_PRINTF("Some negative hex integers: 0x%x, 0x%.12X", -1, -0xdead);
    TEST_PRINTF("Some octal integers: 0%o, 0%o", 8, -27);


    TEST_PRINTF("Big float: %f, big negative float: %f", (float)LLONG_MAX, (float)LLONG_MIN);
    TEST_PRINTF("Some floats: %+.5f, %+9.4f, %5.0f", 3.14159f, -23.8f, -0.23f);

    return 0;
}
