#include "spec.h"

static t_des colour_t_des_filler(int floor_c, int ceiling_c)
{
	t_des filled;

	filled = filled_t_des(0, 0, NULL, NULL, NULL, NULL, NULL, floor_c, ceiling_c);
	return (filled);
}

void	print_diff(char *what, int result, int expected, int first)
{
	if (!first)
		printf(RED "KO\n" RESET);
	printf(UNDER RED"%s :\n" RESET, what);
	printf("Expected = [%d]\n", expected);
	printf("Got      = [%d]\n", result);
}

static void	check_result(t_des result, t_des expected, int *sucess, int *failure)
{
	int fail = 0;

	if (result.floor_c != expected.floor_c)
		print_diff("Floor colour", result.floor_c, expected.floor_c, fail++);
	if (result.ceiling_c != expected.ceiling_c)
		print_diff("Ceiling colour", result.ceiling_c, expected.ceiling_c, fail++);
	if (fail == 0)
		printf(GREEN "OK\n" RESET);
	fail == 0 ? (*sucess)++ : (*failure)++;
}

static void colour_parser_test(char *describe, int returned, t_des current_des, t_des expected, char **line, int *sucess, int *failure)
{
	int response;

	printf("<==== %s ====>\n", describe);
	response = colour_parser(line, &current_des);
	if (response == returned)
		check_result(current_des, expected, sucess, failure);
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
	}
	return ;
}

int main(void)
{
	t_des expected;
	t_des current_des;
	char *line;
	int sucess = 0;
	int failure = 0;

	current_des = colour_t_des_filler(0,0);
	line = "F 255, 255, 255";
	expected = colour_t_des_filler(0xFFFFFF, 0);
	colour_parser_test("Simple black floor", 0, current_des, expected, &line, &sucess, &failure);

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
