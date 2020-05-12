#pragma once

/*
 * Supported printf subset: %(+)(width).(precision)[specifier]
 * Supported format specifiers: 
 * s = string
 * d = integer
 * u = unsigned integer
 * o = octal integer
 * x, X = hex integer
 * f = float
 * e, E = scientific float (not implemented yet)
 * 
 * Note: float precision is limited to 9
 */

void ucprintf(const char *fmt, ...);

/*
 *
 */

void ucscanf(char *fmt, ...);