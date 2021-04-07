#!/bin/bash

LIB_PATH=$1
FUNCTION_NAME=$2
TEST_TYPE=$3

compilation () {
	OPTION=$1
	printf "\n\n\e[1m\e[36m<==== Compiling ====>\n\n\e[0m"
	gcc -Wextra -Wall $OPTION -L../srcs/libft/ -lft -I. -I../srcs/headers ${FUNCTION_NAME}_specs.c utils.c ../srcs/parser/${FUNCTION_NAME}.c -o ${FUNCTION_NAME}_spec
}

run_tests () {
	OPTION=$1
	$OPTION ./${FUNCTION_NAME}_spec
}

clear
printf "\e[1m\e[36m<==== Library ====>\n\n\e[0m"
make libft.a -C ${LIB_PATH}/libft/
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
rm -rf texture_parser_spec texture_parser_spec.dSYM/
