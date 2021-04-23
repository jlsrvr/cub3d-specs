#include "spec.h"

static t_des simple_des_filler(double pos_x, double pos_y, double dir_x, double dir_y, int sprite_cnt, double pos[][3], char *map)
{
	t_des new;

	new.x_res = 1800;
	new.y_res = 1060;
	new.no_path = NULL;
	new.ea_path = NULL;
	new.so_path = NULL;
	new.we_path = NULL;
	new.s_path = NULL;
	new.floor_c = 0xff00ff;
	new.ceiling_c = 0xff01ff;
	new.pos_x = pos_x;
	new.pos_y = pos_y;
	new.dir_x = dir_x;
	new.dir_y = dir_y;
	new.map = (!map ? NULL : ft_split(map, '>'));
	new.sprite_cnt = sprite_cnt;
	if (sprite_cnt > 0)
	{
		new.sprites = malloc(sizeof(*(new.sprites)) * sprite_cnt);
		for (int i = 0; i < sprite_cnt; i++)
		{
			new.sprites[i].pos_x = pos[i][0];
			new.sprites[i].pos_y = pos[i][1];
		}
	}
	else
		new.sprites = NULL;
	return (new);
}

static void	print_diff(char *what, double expected, double result, int first)
{
	if (!first)
		printf(RED "KO\n" RESET);
	printf(UNDER RED"%s :\n" RESET, what);
	printf("Expected = [%f]\n", expected);
	printf("Got      = [%f]\n", result);
}

static void	check_result(t_des result, t_des expected, int *sucess, int *failure)
{
	int fail = 0;

	if (result.pos_x != expected.pos_x)
		print_diff("Player X position", result.pos_x, expected.pos_x, fail++);
	if (result.pos_y != expected.pos_y)
		print_diff("Player Y position", result.pos_y, expected.pos_y, fail++);
	if (result.dir_x != expected.dir_x)
		print_diff("Playe X direction", result.dir_x, expected.dir_x, fail++);
	if (result.dir_y != expected.dir_y)
		print_diff("Player Y direction", result.dir_y, expected.dir_y, fail++);
	if (result.sprite_cnt != expected.sprite_cnt)
	{
		if (!fail)
			printf(RED "KO\n" RESET);
		printf(UNDER RED"Sprite count :\n" RESET);
		printf("Expected = [%d]\n", expected.sprite_cnt);
		printf("Got      = [%d]\n", result.sprite_cnt);
		fail++;
	}
	if (result.sprite_cnt == expected.sprite_cnt)
	{
		if (result.sprites && expected.sprites)
		{
			for (int i = 0; i < result.sprite_cnt; i++)
			{
				if ((result.sprites[i].pos_x != expected.sprites[i].pos_x) || (result.sprites[i].pos_x != expected.sprites[i].pos_x))
				{
					if (!fail)
						printf(RED "KO\n" RESET);
					printf(UNDER RED"Index %d :\n" RESET, i);
					printf("Expected = [%f, %f]\n", expected.sprites[i].pos_x, expected.sprites[i].pos_y);
					printf("Got      = [%f, %f]\n", result.sprites[i].pos_x, result.sprites[i].pos_y);
					fail++;
				}
			}
		}
		else if ((!result.sprites && expected.sprites) || (result.sprites && !expected.sprites))
		{
			printf(UNDER RED "Sprite positions :\n" RESET);
			printf("Expected = %s\n", (expected.sprites ? "Something" : "Nothing"));
			printf("Got      = %s\n", (result.sprites ? "Something" : "Nothing"));
			fail++;
		}
	}
	if (fail == 0)
		printf(GREEN "OK\n" RESET);
	fail == 0 ? (*sucess)++ : (*failure)++;
}


static void map_validator_test(char *describe, int returned, t_des des_in, t_des des_exp, int *sucess, int *failure)
{
	int response;

	printf("<==== %s ====>\n", describe);
	response = map_validator(&des_in);
	if (response == returned)
	{
		printf("<==== Description: ====>\n");
		check_result(des_in, des_exp, sucess, failure);
		printf("<==== Map: ====>\n");
		check_map(des_in.map, des_exp.map, sucess, failure);
	}
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
	}
	free_map(des_in.map);
	free_map(des_exp.map);
	if (des_exp.sprite_cnt > 0)
		free(des_exp.sprites);
	if (des_in.sprite_cnt > 0)
		free(des_in.sprites);
	return ;
}

int main(void)
{
	t_des map_in;
	t_des exp_map_out;
	int sucess = 0;
	int failure = 0;

	/*
	 * N == -1 <== dir_x
	 * E == 1 <== dir_y
	 * S == 1 <== dir_x
	 * W == -1 <== dir_y
	 */
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>10W1>1111");
	exp_map_out = simple_des_filler(2.5, 2.5, 0, -1, 0, NULL, "1111>1001>1001>1111");
	map_validator_test("Simple west validation", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>10E1>1111");
	exp_map_out = simple_des_filler(2.5, 2.5, 0, 1, 0, NULL, "1111>1001>1001>1111");
	map_validator_test("Simple east validation", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>10S1>1111");
	exp_map_out = simple_des_filler(2.5, 2.5, 1, 0, 0, NULL, "1111>1001>1001>1111");
	map_validator_test("Simple south validation", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, " 1111>1001>10W1>1111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, " 1111>1001 >10W1 >1111 ");
	map_validator_test("Simple validation and resize", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, " 1111>   1001>10W1        >1111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, " 1111       >   1001     >10W1        >1111        ");
	map_validator_test("Less simple validation and resize", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1101>1001>10W1>1111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "1101>1001>10W1>1111");
	map_validator_test("Simple invalid map", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1101>1000>10W1>1111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "1101>1000>10W1>1111");
	map_validator_test("Missing a wall", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "111 111>1011101>10000W1>1111111");
	exp_map_out = simple_des_filler(2.5, 5.5, 0, -1, 0, NULL, "1111111>1011101>1000001>1111111");
	map_validator_test("Valid map with space on outside", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "11111111>10000001>10111101>101  101>101  1N1>111  111");
	exp_map_out = simple_des_filler(4.5, 6.5, -1, 0, 0, NULL, "11111111>10000001>10111101>10111101>10111101>11111111");
	map_validator_test("More complex valid map", 0, map_in, exp_map_out, &sucess, &failure);
	double exp_pos[2][3] = {{1.5, 11.5}, {2.5, 20.5}};
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "        1111111111111111111111111>        1002000000110000000000001>        1011000001112000000000001>        1001000000000000000000001>111111111011000001110000000000001>100000000011000001110111111111111>11110111111111011100000010001>11110111111111011101010010001>11000000110101011100000010001>10000000000000001100000010001>10000000000000001101010010001>11000001110101011111011110N0111>11110111 1110101 101111010001>11111111 1111111 111111111111");
	exp_map_out = simple_des_filler(11.5, 26.5, -1, 0, 2, exp_pos, "111111111111111111111111111111111>111111111002000000110000000000001>111111111011000001112000000000001>111111111001000000000000000000001>111111111011000001110000000000001>100000000011000001110111111111111>111101111111110111000000100011111>111101111111110111010100100011111>110000001101010111000000100011111>100000000000000011000000100011111>100000000000000011010100100011111>110000011101010111110111100011111>111101111111010111011110100011111>111111111111111111111111111111111");
	map_validator_test("Valid map found in pdf", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0,  0, NULL,"11111111>10000001>10011101>101  101>101  1N1>111  111");
	exp_map_out = simple_des_filler(0, 0, 0, 0,  0, NULL,"11111111>10000001>10011101>101  101>101  1N1>111  111");
	map_validator_test("Incomplet inside wall", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "11111111>10000001>10111101>101  101>100  1N1>111  111");
	exp_map_out =  simple_des_filler(0, 0, 0, 0, 0, NULL, "11111111>10000001>10111101>101  101>100  1N1>111  111");
	map_validator_test("Incomplet corridor", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "11111111>10000001>10111101>101  101>101  1N1>111");
	exp_map_out =  simple_des_filler(0, 0, 0, 0, 0, NULL, "11111111>10000001>10111101>101  101>101  1N1>111     ");
	map_validator_test("N instead of wall", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>1WW1>1111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>1WW1>1111");
	map_validator_test("Simple map with two positions", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>1001>1111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "1111>1001>1001>1111");
	map_validator_test("Simple map with no position", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "        1111111111111111111111111>        1000000000110000000N00001>        1011000001110000002000001>        1001000000000000000000001>11111111101100000111E000000000001>100000000011000001110111111111111>11110111111111011100000010001>11110111111111011101010010001>11000000110101011100000010001>10002000000000001100000010001>10000000000000001101010010001>11000001110101011111011110N0111>11110111 1110101 101111010001>11111111 1111111 111111111111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "        1111111111111111111111111>        1000000000110000000N00001>        1011000001110000002000001>        1001000000000000000000001>11111111101100000111E000000000001>100000000011000001110111111111111>11110111111111011100000010001    >11110111111111011101010010001    >11000000110101011100000010001    >10002000000000001100000010001    >10000000000000001101010010001    >11000001110101011111011110N0111  >11110111 1110101 101111010001    >11111111 1111111 111111111111    ");
	map_validator_test("Map found in pdf but with added player positions", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "        1111111111111111111111111>        1000000000110000000000001>        1011000001110000002000001>        1001000000000000000000001>111111111011000001110000000000001>100000000011000001110111111111111>11110111111111011100000010001>11110111111111011101010010001>11000000110101011100000010001>10002000000000001100000010001>10000000000000001101010010001>1100000111010101111101111000111>11110111 1110101 101111010001>11111111 1111111 111111111111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "        1111111111111111111111111>        1000000000110000000000001>        1011000001110000002000001>        1001000000000000000000001>111111111011000001110000000000001>100000000011000001110111111111111>11110111111111011100000010001    >11110111111111011101010010001    >11000000110101011100000010001    >10002000000000001100000010001    >10000000000000001101010010001    >1100000111010101111101111000111  >11110111 1110101 101111010001    >11111111 1111111 111111111111    ");
	map_validator_test("Map found in pdf but removed player positions", 1, map_in, exp_map_out, &sucess, &failure);
	double exp_posi[1][3] = {{1.5, 3.5}};
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "111111111>100020001>101111101>1N1   101>101   101>101   101>101111101>100000001>111111111");
	exp_map_out =  simple_des_filler(3.5, 1.5, -1, 0, 1, exp_posi, "111111111>100020001>101111101>101111101>101111101>101111101>101111101>100000001>111111111");
	map_validator_test("Hole inside valid map", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = simple_des_filler(0, 0, 0, 0, 0, NULL, "111111111>100000001>100111101>1N1   101>101   101>101   101>101111101>100000001>111111111");
	exp_map_out = simple_des_filler(0, 0, 0, 0, 0, NULL, "111111111>100000001>100111101>1N1   101>101   101>101   101>101111101>100000001>111111111");
	map_validator_test("Unclosed inner wall", 1, map_in, exp_map_out, &sucess, &failure);

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
