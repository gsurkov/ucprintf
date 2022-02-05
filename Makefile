all:
	@gcc -std=c99 -g -Wall -Wextra -fpack-struct -o printf test_printf.c ucprintf.c
	@./printf
