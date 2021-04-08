#include "spec.h"

static t_des texture_t_des_filler(char *no_path, char *ea_path, char *so_path, char *we_path, char *s_path)
{
	t_des filled;

	filled = filled_t_des(0, 0, no_path, ea_path, so_path, we_path, s_path, 0xffffff, 0x000000);
	return (filled);
}

void	print_diff(char *what, char *result, char *expected, int first)
{
	if (!first)
		printf(RED "KO\n" RESET);
	printf(UNDER RED"%s :\n" RESET, what);
	printf("Expected = [%s]\n", expected);
	printf("Got      = [%s]\n", result);
}

static int comp_string(char *str1, char *str2)
{
	if (!str1 && !str2)
		return (0);
	if (!str1 || !str2)
		return (1);
	return (strcmp(str1, str2));
}

static void	check_result(t_des result, t_des expected, int *sucess, int *failure)
{
	int fail = 0;

	if (comp_string(result.no_path, expected.no_path))
		print_diff("North filepath", result.no_path, expected.no_path, fail++);
	if (comp_string(result.ea_path, expected.ea_path))
		print_diff("East filepath", result.ea_path, expected.ea_path, fail++);
	if (comp_string(result.so_path, expected.so_path))
		print_diff("South filepath", result.so_path, expected.so_path, fail++);
	if (comp_string(result.we_path, expected.we_path))
		print_diff("West filepath", result.we_path, expected.we_path, fail++);
	if (comp_string(result.s_path, expected.s_path))
		print_diff("Sprite filepath", result.s_path, expected.s_path, fail++);
	if (fail == 0)
		printf(GREEN "OK\n" RESET);
	fail == 0 ? (*sucess)++ : (*failure)++;
}

void free_result(t_des description)
{
	free(description.no_path);
	free(description.ea_path);
	free(description.so_path);
	free(description.we_path);
	free(description.s_path);
}

static void texture_parser_test(char *describe, int returned, t_des current_des, t_des expected, char **line, int *sucess, int *failure)
{
	int response;

	printf("<==== %s ====>\n", describe);
	response = texture_parser(line, &current_des);
	if (response == returned)
		check_result(current_des, expected, sucess, failure);
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
	}
	free_result(current_des);
	return ;
}

int main(void)
{
	t_des expected;
	t_des current_des;
	char *line;
	int sucess = 0;
	int failure = 0;

	current_des = texture_t_des_filler(NULL, NULL, NULL, NULL, NULL);
	line = "NO ./a/file_path.c";
	expected = texture_t_des_filler("./a/file_path.c", NULL, NULL, NULL, NULL);
	texture_parser_test("Simple northern texture", 0, current_des, expected, &line, &sucess, &failure);
	line = "EA ./a/file_path.c";
	expected = texture_t_des_filler(NULL, "./a/file_path.c", NULL, NULL, NULL);
	texture_parser_test("Simple eastern texture", 0, current_des, expected, &line, &sucess, &failure);
	line = "SO ./a/file_path.c";
	expected = texture_t_des_filler(NULL, NULL, "./a/file_path.c", NULL, NULL);
	texture_parser_test("Simple southern texture", 0, current_des, expected, &line, &sucess, &failure);
	line = "WE ./a/file_path.c";
	expected = texture_t_des_filler(NULL, NULL, NULL, "./a/file_path.c", NULL);
	texture_parser_test("Simple western texture", 0, current_des, expected, &line, &sucess, &failure);
	line = "S ./a/file_path.c";
	expected = texture_t_des_filler(NULL, NULL, NULL, NULL, "./a/file_path.c");
	texture_parser_test("Simple sprite texture", 0, current_des, expected, &line, &sucess, &failure);
	line = "S    	 ./a/file_path.c";
	expected = texture_t_des_filler(NULL, NULL, NULL, NULL, "./a/file_path.c");
	texture_parser_test("Multiple spaces between identifier and path", 0, current_des, expected, &line, &sucess, &failure);
	current_des = texture_t_des_filler(ft_strdup("./a/file_path.c"), NULL, NULL, NULL, NULL);
	line = "NO ./a/file_path.c";
	expected = texture_t_des_filler("|duplicate|", NULL, NULL, NULL, NULL);
	texture_parser_test("Path has already been filled", 1, current_des, expected, &line, &sucess, &failure);

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
