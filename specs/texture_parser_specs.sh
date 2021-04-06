#!/bin/bash

compilation () {
	printf "\n\n\e[1m\e[36m<==== Compiling ====>\n\n\e[0m"
	gcc -Wextra -Wall $1 -L../srcs/libft/ -lft -I. -I../srcs/headers texture_parser_specs.c utils.c ../srcs/parser/texture_parser.c -o texture_parser_spec
}

run_tests () {
	$1 ./texture_parser_spec
}

printf "\e[1m\e[36m<==== Library ====>\n\n\e[0m"
make libft.a -C $1/libft/
if [ -z $2 ]
then
	compilation
	printf "\n\n\e[1m\e[36m<==== Basic tests====>\n\n\e[0m"
	run_tests
else
	if [ $2 = "sanitize" ]; then
		compilation "-fsanitize=address"
		printf "\n\n\e[1m\e[36m<==== Sanitised tests====>\n\n\e[0m"
		run_tests
	elif [ $2 = "valgrind" ]; then
		compilation
		printf "\n\n\e[1m\e[36m<==== Valgrind tests====>\n\n\e[0m"
		run_tests "valgrind  --leak-check=full"
	else
		echo $2 not an option!!!
		compilation
		printf "\n\n\e[1m\e[36m<==== Basic tests====>\n\n\e[0m"
		run_tests
	fi
fi
rm -rf texture_parser_spec texture_parser_spec.dSYM/
