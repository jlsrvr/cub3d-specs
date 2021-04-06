#!/bin/bash

make libft.a -C $1/libft/
gcc -Wextra -Wall -L../srcs/libft/ -lft -I. -I../srcs/headers texture_parser_specs.c ../srcs/parser/texture_parser.c -o texture_parser_spec
valgrind  --leak-check=full ./texture_parser_spec
rm -rf texture_parser_spec texture_parser_spec.dSYM/
