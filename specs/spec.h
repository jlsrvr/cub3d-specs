#ifndef SPEC_H
# define SPEC_H

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include "cub3d.h"
# include "parser.h"


# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PINK	"\e[35m"
# define CYAN	"\e[36m"
# define BOLD	"\e[1m"
# define UNDER	"\e[4m"
# define BLINK	"\e[5m"
# define RESET  "\e[0m"

t_des filled_t_des(int x_res, int y_res, char *no_path, char *ea_path, char *so_path, char *we_path, char *s_path, int floor_c, int ceiling_c);
#endif
