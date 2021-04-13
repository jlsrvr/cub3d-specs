#!/bin/bash

TEST_TYPE=$1

compilation () {
	OPTION=$1
	printf "\n\n\e[1m\e[36m<==== Compiling ====>\n\n\e[0m"
	gcc -Wextra -Wall $OPTION -L../srcs/libft/ -lft -I. -I../srcs/headers cub_validator_specs.c utils.c ../srcs/cub_utils.c ../srcs/validator/*.c -o cub_validator_spec
}

run_tests () {
	OPTION=$1
	$OPTION ./cub_validator_spec
}

clear
printf "\e[1m\e[36m<==== Library ====>\n\n\e[0m"
make libft.a -C ../srcs/libft/
if [ -z $TEST_TYPE ]
then
	compilation
	printf "\n\n\e[1m\e[36m<==== Basic tests====>\n\n\e[0m"
	run_tests
else
	if [ $TEST_TYPE = "sanitize" ]; then
		compilation "-fsanitize=address"
		printf "\n\n\e[1m\e[36m<==== Sanitised tests====>\n\n\e[0m"
		run_tests
	elif [ $TEST_TYPE = "debug" ]; then
		compilation "-g"
		printf "\n\n\e[1m\e[36m<==== Debugging mode with lldb====>\n\n > b name_of_break_point then > r to run program then > gui to access interface\n\e[0m"
		run_tests "lldb"
	elif [ $TEST_TYPE = "valgrind" ]; then
		compilation
		printf "\n\n\e[1m\e[36m<==== Valgrind tests====>\n\n\e[0m"
		run_tests "valgrind  --leak-check=full"
	else
		echo $TEST_TYPE not an option!!!
		compilation
		printf "\n\n\e[1m\e[36m<==== Basic tests====>\n\n\e[0m"
		run_tests
	fi
fi
rm -rf cub_validator_spec cub_validator_spec.dSYM/
