#include "spec.h"

static t_des resolution_t_des_filler(int x_res, int y_res)
{
	t_des filled;

	filled = filled_t_des(x_res, y_res, NULL, NULL, NULL, NULL, NULL, 0, 0);
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

	if (result.x_res != expected.x_res)
		print_diff("X axis resolution", result.x_res, expected.x_res, fail++);
	if (result.y_res != expected.y_res)
		print_diff("Y axis resolution", result.y_res, expected.y_res, fail++);
	if (fail == 0)
		printf(GREEN "OK\n" RESET);
	fail == 0 ? (*sucess)++ : (*failure)++;
}

static void resolution_parser_test(char *describe, int returned, t_des current_des, t_des expected, char **line, int *sucess, int *failure)
{
	int response;

	printf("<==== %s ====>\n", describe);
	response = resolution_parser(line, &current_des);
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

	current_des = resolution_t_des_filler(0,0);
	line = "R 1900 1800";
	expected = resolution_t_des_filler(1900, 1800);
	resolution_parser_test("Simple resolution", 0, current_des, expected, &line, &sucess, &failure);
	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
