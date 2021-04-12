#include "spec.h"

t_des filled_t_des(int x_res, int y_res, char *no_path, char *ea_path, char *so_path, char *we_path, char *s_path, int floor_c, int ceiling_c, char *map)
{
	t_des new;

	new.x_res = x_res;
	new.y_res = y_res;
	new.no_path = no_path;
	new.ea_path = ea_path;
	new.so_path = so_path;
	new.we_path = we_path;
	new.s_path = s_path;
	new.floor_c = floor_c;
	new.ceiling_c = ceiling_c;
	new.map = (!map ? NULL : ft_split(map, '>'));
	return (new);
}

int comp_string(char *str1, char *str2)
{
	if (!str1 && !str2)
		return (0);
	if (!str1 || !str2)
		return (1);
	return (strcmp(str1, str2));
}

void	check_map(t_des result, t_des expected, int *sucess, int *failure)
{
	int x = 0;
	char **map_res;
	char **map_exp;

	map_res = result.map;
	map_exp = expected.map;
	if ((!map_res && map_exp) || (map_res && !map_exp))
	{
		printf(RED "KO\n" RESET);
		printf(UNDER RED "Map pointer :\n" RESET);
		printf("Expected = %s\n", (map_exp ? "Something" : "Nothing"));
		printf("Got      = %s\n", (map_res ? "Something" : "Nothing"));
		(*failure)++;
		return ;
	}
	if (map_res && map_exp)
	{
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
	}
	printf(GREEN "OK\n" RESET);
	(*sucess)++;
}

void free_map(char **map)
{
	int index = 0;

	if (map)
		while (map[index])
			free(map[index++]);
	free(map);
}
