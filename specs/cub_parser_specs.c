#include "spec.h"

void	print_diff(char *what, char *str_result, char *str_expected, int result, int expected, int first, int type)
{
	if (!first)
		printf(RED "KO\n" RESET);
	printf(UNDER RED"%s :\n" RESET, what);
	if (type == 1)
	{
		printf("Expected = [%d]\n", expected);
		printf("Got      = [%d]\n", result);
	}
	else if (type == 2)
	{
		printf("Expected = [%s]\n", str_expected);
		printf("Got      = [%s]\n", str_result);
	}
	else
	{
		printf("Expected = [%X]\n", expected);
		printf("Got      = [%X]\n", result);
	}
}

static int comp_string(char *str1, char *str2)
{
	if (!str1 && !str2)
		return (0);
	if (!str1 || !str2)
		return (1);
	return (strcmp(str1, str2));
}

static void	check_result(t_des *result, t_des *expected, int *sucess, int *failure)
{
	int fail = 0;
	if ((expected && !result) ||(!expected && result) )
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%p]\n", expected);
		printf("Got      = [%p]\n", result);
		fail++;
	}
	if (expected && result)
	{
		if (result->x_res != expected->x_res)
			print_diff("X resolution", NULL, NULL, result->x_res, expected->x_res, fail++, 1);
		if (result->y_res != expected->y_res)
			print_diff("Y resolution", NULL, NULL, result->y_res, expected->y_res, fail++, 1);
		if (comp_string(result->no_path, expected->no_path))
			print_diff("North filepath", result->no_path, expected->no_path, 0, 0, fail++, 2);
		if (comp_string(result->ea_path, expected->ea_path))
			print_diff("East filepath", result->ea_path, expected->ea_path, 0, 0, fail++, 2);
		if (comp_string(result->so_path, expected->so_path))
			print_diff("South filepath", result->so_path, expected->so_path, 0, 0, fail++, 2);
		if (comp_string(result->we_path, expected->we_path))
			print_diff("West filepath", result->we_path, expected->we_path, 0, 0, fail++, 2);
		if (comp_string(result->s_path, expected->s_path))
			print_diff("Sprite filepath", result->s_path, expected->s_path, 0, 0, fail++, 2);
		if (result->floor_c != expected->floor_c)
			print_diff("Floor colour", NULL, NULL, result->floor_c, expected->floor_c, fail++, 3);
		if (result->ceiling_c != expected->ceiling_c)
			print_diff("Ceiling colour", NULL, NULL, result->ceiling_c, expected->ceiling_c, fail++, 3);
	}
	if (fail == 0)
		printf(GREEN "OK\n" RESET);
	fail == 0 ? (*sucess)++ : (*failure)++;
}

void free_result(t_des *description)
{
	if (!description)
		return ;
	free(description->no_path);
	free(description->ea_path);
	free(description->so_path);
	free(description->we_path);
	free(description->s_path);
	free(description);
}

static void texture_parser_test(char *describe, char *path, int returned, t_des *expected, int *sucess, int *failure)
{
	int response;
	int fd;
	t_des *description;

	printf("<==== %s ====>\n", describe);

	description = NULL;
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		printf(RED "KO \t");
		printf("%s : %s \n\n"RESET, path, strerror(errno));
		(*failure)++;
		return ;
	}
	response = cub_parser(fd, &description);
	if (response == returned)
		check_result(description, expected, sucess, failure);
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
		(*failure)++;
	}
	free_result(description);
	close(fd);
	return ;
}

int main(void)
{
	t_des expected;
	int sucess = 0;
	int failure = 0;

	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000);
	texture_parser_test("Simple well formated file", "files/well_formated.cub", 0, &expected, &sucess, &failure);
	texture_parser_test("Well formated file shuffled", "files/well_formated_shuffled.cub", 0, &expected, &sucess, &failure);
	texture_parser_test("Well formated with empty lines", "files/WF_with_empty_lines.cub", 0, &expected, &sucess, &failure);
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", -1, -2);
	texture_parser_test("Floor color wrong format", "files/invalid_floor_c.cub", 1, &expected, &sucess, &failure);
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "|duplicate|", NULL, -2, -2);
	texture_parser_test("Duplicate west path", "files/duplicate_we_path.cub", 1, &expected, &sucess, &failure);
	expected = filled_t_des(-1, -1, NULL, NULL, NULL, NULL, NULL, -2, -2);
	texture_parser_test("Resolution in the wrong format", "files/wrong_format_resolution.cub", 1, &expected, &sucess, &failure);
	expected = filled_t_des(0, 0, NULL, NULL, NULL, NULL, NULL, -2, -2);
	texture_parser_test("Wrong identifier at start", "files/unknown_identifier.cub", 1, &expected, &sucess, &failure);
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, -2);
	texture_parser_test("Missing element", "files/missing_ceiling_c.cub", 1, &expected, &sucess, &failure);
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", -2, -2);
	texture_parser_test("Format ok, until line with a few space chars but then random", "files/with_line_starting_space_then_random.cub", 1, &expected, &sucess, &failure);

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
