all:
	@gcc -std=c99 -g -Wall -Wextra -o printf test_printf.c ucprintf.c
	@./printf
