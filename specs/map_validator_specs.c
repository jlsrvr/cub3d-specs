#include "spec.h"

static void map_validator_test(char *describe, int returned, char *str_map_in, char *str_exp_map_out, int *sucess, int *failure)
{
	int response;
	char **map_in;
	char **exp_map_out;

	map_in = ft_split(str_map_in, '>');
	exp_map_out = ft_split(str_exp_map_out, '>');
	printf("<==== %s ====>\n", describe);
	response = map_validator(&map_in);
	if (response == returned)
		check_map(map_in, exp_map_out, sucess, failure);
	else
	{
		printf(RED "KO\n" RESET);
		printf("Expected = [%d]\n", returned);
		printf("Got      = [%d]\n", response);
	}
	free_map(map_in);
	free_map(exp_map_out);
	return ;
}

int main(void)
{
	char *map_in;
	char *exp_map_out;
	int sucess = 0;
	int failure = 0;

	map_in = "1111>1001>10W1>1111";
	exp_map_out = "1111>1001>10W1>1111";
	map_validator_test("Simple validation", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = " 1111>1001>10W1>1111";
	exp_map_out = " 1111>1001 >10W1 >1111 ";
	map_validator_test("Simple validation and resize", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = " 1111>   1001>10W1        >1111";
	exp_map_out = " 1111       >   1001     >10W1        >1111        ";
	map_validator_test("Less simple validation and resize", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = "1101>1001>10W1>1111";
	exp_map_out = map_in;
	map_validator_test("Simple invalid map", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = "1111>1000>10W1>1111";
	exp_map_out = map_in;
	map_validator_test("Missing a wall", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = "111 111>1011101>10000W1>1111111";
	exp_map_out = map_in;
	map_validator_test("Valid map with space on outside", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = "11111111>10000001>10111101>101  101>101  1N1>111  111";
	exp_map_out = map_in;
	map_validator_test("More complex valid map", 0, map_in, exp_map_out, &sucess, &failure);
	map_in = "11111111>10000001>10011101>101  101>101  1N1>111  111";
	exp_map_out = map_in;
	map_validator_test("Incomplet inside wall", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = "11111111>10000001>10111101>101  101>100  1N1>111  111";
	exp_map_out = map_in;
	map_validator_test("Incomplet corridor", 1, map_in, exp_map_out, &sucess, &failure);
	map_in = "11111111>10000001>10111101>101  101>101  1N1>111";
	exp_map_out =  "11111111>10000001>10111101>101  101>101  1N1>111    ";
	map_validator_test("N instead of wall", 1, map_in, exp_map_out, &sucess, &failure);

	printf("\t%d success out of %d tests\n", sucess, (sucess + failure));
	return (0);
}
