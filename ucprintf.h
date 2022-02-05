#pragma once

/*
 * Supported printf subset: %(+)(width).(precision)(size)[specifier]
 * Supported format specifiers: 
 * s = string
 * d,i = integer
 * u = unsigned integer
 * o = octal integer
 * x, X = hex integer
 * f = float
 *
 * Supported size modifiers:
 * hh - char
 * h - short
 * l - long
 * ll - long long
 * 
 * Note: float precision is limited to 9.
 */

void ucprintf(const char *fmt, ...);

/*
 *
 */

void ucscanf(char *fmt, ...);
