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
	check_map(result->map, expected->map, sucess, failure);
}

void free_result(t_des *description)
{
	if (!description)
		return ;
}

static void cub_validator_test(char *describe, int returned, t_des *des_in, t_des *expected, int *sucess, int *failure)
{
	int response;

	printf("<==== %s ====>\n", describe);
	if((response = cub_validator(des_in)))
		printf("\n");
	if (response == returned)
		check_result(des_in, expected, sucess, failure);
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
		(*failure)++;
	}
	free_map(des_in->map);
	free_map(expected->map);
	return ;
}

int main(void)
{
	t_des expected;
	t_des des_in;
	int sucess = 0;
	int failure = 0;
	char *describe;

	describe = "Simple valid description";
	des_in = filled_t_des(1900, 1800, "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	expected = filled_t_des(1900, 1800, "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_validator_test(describe, 0, &des_in, &expected, &sucess, &failure);
	describe = "Invalid textures";
	des_in = filled_t_des(1900, 1800, "./files/test.xpm", "./files/tes.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	expected = filled_t_des(1900, 1800, "./files/test.xpm", "./files/tes.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111>1001>1W01>1111");
	cub_validator_test(describe, 1, &des_in, &expected, &sucess, &failure);
	describe = "Invalid map";
	des_in = filled_t_des(1900, 1800, "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111>1001>1W01>1011");
	expected = filled_t_des(1900, 1800, "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111>1001>1W01>1011");
	cub_validator_test(describe, 1, &des_in, &expected, &sucess, &failure);
	describe = "Simple valid description change map";
	des_in = filled_t_des(1900, 1800, "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111   >1001>1W01>1111");
	expected = filled_t_des(1900, 1800, "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", 0xFFFFFF, 0x000000, "1111111>1001111>1W01111>1111111");
	cub_validator_test(describe, 0, &des_in, &expected, &sucess, &failure);
	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
