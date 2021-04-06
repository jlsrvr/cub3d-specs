#include "spec.h"

t_des filled_t_des(int x_res, int y_res, char *no_path, char *ea_path, char *so_path, char *we_path, char *s_path, int floor_c, int ceiling_c)
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
	new.map = NULL;
	return (new);
}
