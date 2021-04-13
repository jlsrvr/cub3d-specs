#include "spec.h"

static t_des texture_t_des_filler(char *no_path, char *ea_path, char *so_path, char *we_path, char *s_path)
{
	t_des filled;

	filled = filled_t_des(0, 0, no_path, ea_path, so_path, we_path, s_path, 0xffffff, 0x000000, NULL);
	return (filled);
}

static void texture_validator_test(char *describe, int returned, t_des description, int *sucess, int *failure)
{
	int response;

	printf("<==== %s ====>\n", describe);
	response = textures_validator(&description);
	if (response == returned)
	{
		(*sucess)++;
		printf(GREEN"OK\n" RESET);
	}
	else
	{
		(*failure)++;
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
	}
	return ;
}

int main(void)
{
	t_des description;
	int sucess = 0;
	int failure = 0;

	description = texture_t_des_filler(NULL, NULL, NULL, NULL, NULL);
	texture_validator_test("All invalid paths", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm");
	texture_validator_test("All valid paths", 0, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./fi/test.xpm");
	texture_validator_test("Last path invalid", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/");
	texture_validator_test("Last path is a directory", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/fake_colour.cub");
	texture_validator_test("Last path has wrong extension", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm.else");
	texture_validator_test("Invalid last path but file that opens and extension in name", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./fis/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm");
	texture_validator_test("1st path invalid", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./fes/test.xpm", "./files/test.xpm", "./files/test.xpm", "./files/test.xpm");
	texture_validator_test("2nd path invalid", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/tet.xpm", "./files/test.xpm", "./files/test.xpm");
	texture_validator_test("3rd path invalid", 1, description, &sucess, &failure);
	description = texture_t_des_filler("./files/test.xpm", "./files/test.xpm", "./files/test.xpm", "./fes/test.xpm", "./files/test.xpm");
	texture_validator_test("4th path invalid", 1, description, &sucess, &failure);

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
