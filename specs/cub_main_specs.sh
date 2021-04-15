RED="\033[31m"
GREEN="\033[32m"
PINK="\033[35m"
CYAN="\033[36m"
BOLD="\033[1m"
UNDER="\033[4m"
BLINK="\033[5m"
RESET="\033[m"


test () {
	describe=$1
	file_path="./files/cub_main_files/${2}"
	if [ -z $2 ]; then
		test_file=""
	else
		test_file="${file_path}.cub"
	fi

	printf "\n\n${UNDER}${describe} :\n\n${RESET}"
	../srcs/cub3d ${test_file} > ${file_path}_test_output
	diff -u ${file_path}_output ${file_path}_test_output
	RESULT=$?
	if [ ${RESULT} -eq 0 ]
	then
		printf "${GREEN}OK${RESET} see behaviour at: ${file_path}_output\n"
		rm -f ${file_path}_test_output
	else
		echo "${RED}KO${RESET}"
	fi
}

clear
printf "${BOLD}${CYAN}<==== Compiling project ====>\n\n${RESET}"
make cub3d -C ../srcs
echo
echo

printf "${BOLD}${CYAN}<==== Running basic error tests ====>${RESET}"

test "Incorrect character in the map" valid_until_parsing_map
test "Invalid .cub file" invalid.cub
test "No path given"
test "All good parsing but invalid textures" parsing_ok_textures_invalid
test "All good parsing but invalid map" all_good_map_invalid


