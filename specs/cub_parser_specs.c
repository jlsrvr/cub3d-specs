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

static void	check_result(t_des *result, t_des *expected, int *sucess, int *failure)
{
	int fail = 0;
	if ((expected && !result) ||(!expected && result) )
	{
		printf(RED "KO\n" RESET);
		printf(UNDER RED "Pointer :\n" RESET);
		printf("Expected = %s\n", (expected ? "Something" : "Nothing"));
		printf("Got      = %s\n", (result ? "Something" : "Nothing"));
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
	if (fail != 0)
	{
		(*failure)++;
		return ;
	}
	check_map(*result, *expected, sucess, failure);
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
	free_map(description->map);
	free(description);
}

static void cub_parser_test(char *describe, char *path, int returned, t_des *expected, int *sucess, int *failure)
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
	free_map(expected->map);
	close(fd);
	return ;
}

int main(void)
{
	t_des expected;
	int sucess = 0;
	int failure = 0;
	char *describe;

	describe = "Simple well formated file";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_parser_test(describe, "files/well_formated.cub", 0, &expected, &sucess, &failure);
	describe = "Well formated file shuffled";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_parser_test(describe, "files/well_formated_shuffled.cub", 0, &expected, &sucess, &failure);
	describe = "Well formated with empty lines";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_parser_test(describe, "files/WF_with_empty_lines.cub", 0, &expected, &sucess, &failure);
	describe = "Floor color wrong format";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", -1, -2, NULL);
	cub_parser_test(describe, "files/invalid_floor_c.cub", 1, &expected, &sucess, &failure);
	describe = "Duplicate west path";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "|duplicate|", NULL, -2, -2, NULL);
	cub_parser_test(describe, "files/duplicate_we_path.cub", 1, &expected, &sucess, &failure);
	describe = "Resolution in the wrong format";
	expected = filled_t_des(-1, -1, NULL, NULL, NULL, NULL, NULL, -2, -2, NULL);
	cub_parser_test(describe, "files/wrong_format_resolution.cub", 1, &expected, &sucess, &failure);
	describe = "Wrong identifier at start";
	expected = filled_t_des(0, 0, NULL, NULL, NULL, NULL, NULL, -2, -2, NULL);
	cub_parser_test(describe, "files/unknown_identifier.cub", 1, &expected, &sucess, &failure);
	describe = "Fake resolution at start";
	expected = filled_t_des(-1, -1, NULL, NULL, NULL, NULL, NULL, -2, -2, NULL);
	cub_parser_test(describe, "files/fake_resolution.cub", 1, &expected, &sucess, &failure);
	describe = "Fake colour at start";
	expected = filled_t_des(0, 0, NULL, NULL, NULL, NULL, NULL, -1, -2, NULL);
	cub_parser_test(describe, "files/fake_colour.cub", 1, &expected, &sucess, &failure);
	describe = "Fake texture at start";
	expected = filled_t_des(0, 0, NULL, NULL, NULL, NULL, NULL, -2, -2, NULL);
	cub_parser_test(describe, "files/fake_texture.cub", 1, &expected, &sucess, &failure);
	describe = "Missing element";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, -2, NULL);
	cub_parser_test(describe, "files/missing_ceiling_c.cub", 1, &expected, &sucess, &failure);
	describe = "Format ok, until line with a few space chars but then random";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", -2, -2, NULL);
	cub_parser_test(describe, "files/with_line_starting_space_then_random.cub", 1, &expected, &sucess, &failure);
	describe = "All good except empty line in map";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01");
	cub_parser_test(describe, "files/easy_empty_line_in_map.cub", 0, &expected, &sucess, &failure);
	describe = "All good except line with only space characters in map";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01");
	cub_parser_test(describe, "files/line_only_space_chars_in_map.cub", 0, &expected, &sucess, &failure);
	describe = "All good, empty lines after the map";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_parser_test(describe, "files/empty_lines_after_map.cub", 0, &expected, &sucess, &failure);
	describe = "Complex map";
	expected = filled_t_des(1920, 1080, "./path_to_the_north_texture", "./path_to_the_east_texture", "./path_to_the_south_texture", "./path_to_the_west_texture", "./path_to_the_sprite_texture", 0xDC6400, 0xE11E00, "    	1111111111111111111111111>    	1000000000110000000000001>    	1011000001110000002000001>    	1001000000000000000000001>111111111011000001110000000000001>100000000011000001110111111111111>11110111111111011100000010001>11110111111111011101010010001>11000000110101011100000010001>10002000000000001100000010001>10000000000000001101010010001>11000001110101011111011110N0111>11110111 1110101 101111010001>11111111 1111111 111111111111>");
	cub_parser_test(describe, "files/harder.cub", 0, &expected, &sucess, &failure);
	describe = "Simple but invalid first line of map";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, NULL);
	cub_parser_test(describe, "files/invalid_first_line_map.cub", 1, &expected, &sucess, &failure);
	describe = "Simple but EOF on last line of map";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_parser_test(describe, "files/no_newline_after_map.cub", 0, &expected, &sucess, &failure);
	describe = "Simple but invalid third line map";
	expected = filled_t_des(1900, 1800, "./a/no_path.c", "./a/ea_path.c", "./a/so_path.c", "./a/we_path.c", "./a/s_path.c", 0xFFFFFF, 0x000000, "1111>1001");
	cub_parser_test(describe, "files/invalid_third_line_map.cub", 1, &expected, &sucess, &failure);
	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
