#include "spec.h"

static t_des map_t_des_filler(char *map)
{
	t_des new;

	new.x_res = 0;
	new.y_res = 0;
	new.no_path = NULL;
	new.ea_path = NULL;
	new.so_path = NULL;
	new.we_path = NULL;
	new.s_path = NULL;
	new.floor_c = -2;
	new.ceiling_c = -2;
	new.map = (!map ? NULL : ft_split(map, '>'));
	return (new);
}

static void free_map(char **map)
{
	int index = 0;

	if (map)
		while (map[index])
			free(map[index++]);
	free(map);
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
	int x = 0;
	char **map_res;
	char **map_exp;

	map_res = result.map;
	map_exp = expected.map;
	if ((!map_res && map_exp) || (map_res && !map_exp))
	{
		printf(RED "KO\n" RESET);
		printf(UNDER RED "Pointer :\n" RESET);
		printf("Expected = %s\n", (map_exp ? "Something" : "Nothing"));
		printf("Got      = %s\n", (map_res ? "Something" : "Nothing"));
		(*failure)++;
		return ;
	}
	while (map_res[x] && map_exp[x])
	{
		if (comp_string(map_res[x], map_exp[x]))
		{
			printf(RED "KO\n" RESET);
			printf(UNDER RED "Line [%d]\n" RESET, (x + 1));
			printf("Expected = [%s]\n", map_exp[x]);
			printf("Got      = [%s]\n", map_res[x]);
			(*failure)++;
			return ;
		}
		x++;
	}
	printf(GREEN "OK\n" RESET);
	(*sucess)++;
}

static void map_parser_test(char *describe, int returned, char *current, char *expected, char **line, int *sucess, int *failure)
{
	int response;
	t_des expected_des;
	t_des current_des;

	expected_des = map_t_des_filler(expected);
	current_des = map_t_des_filler(current);
	printf("<==== %s ====>\n", describe);
	response = map_parser(line, &current_des);
	if (response == returned)
		check_result(current_des, expected_des, sucess, failure);
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
	}
	free_map(current_des.map);
	free_map(expected_des.map);
	return ;
}

int main(void)
{
	char *line;
	int sucess = 0;
	int failure = 0;

	line = "1111";
	map_parser_test("Valid first line of a map", 0, NULL, "1111", &line, &sucess, &failure);
	line = "1113";
	map_parser_test("Second line but it contains invalid number", 1, "1111", "1111", &line, &sucess, &failure);
	line = "1111    -";
	map_parser_test("Second line but it contains invalid char", 1, "1111", "1111", &line, &sucess, &failure);
	line = "13EW";
	map_parser_test("Second line but it contains 2 player orientations", 1, "1111", "1111", &line, &sucess, &failure);
	line = "10E1";
	map_parser_test("Valid second line", 0, "1111", "1111>10E1", &line, &sucess, &failure);


	/*line = "1001";
	expected = map_t_des_filler("1111>1001");
	current_des = map_t_des_filler("1111");
	map_parser_test("First line of a map", 0, current_des, expected, &line, &sucess, &failure);*/

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
