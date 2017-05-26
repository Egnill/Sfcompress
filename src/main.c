#include "sfc.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("An error has occured!\nERROR: Missing the instruction for the program!\nHint: Please, try again!\n-c   Compressing\n-d   Decompressing\n");
	} else {
		if (argc < 3) {
			printf("An error has occured!\nERROR: Missing input file name!\nHint: Please, try again!\n");
		} else {
			if (argc < 4) {
				printf("An error has occured!\nERROR: Missing binary file name!\nHint: Please, look up *.sf file!\n");
			}
		}
	}
	
	if ((argv[1][1] != 'c' || argv[1][1] != 'd') && argv[1][0] != '-') {
		printf("An error has occured!\nERROR: Wrong instruction!\nHint: Please, check your input value!\n-c   Compressing\n-d   Decompressing\n");
	}

	if (argv[1][1] == 'c') {
		compress(argv[2], argv[3]);
		printf("Completed!\nSTATUS: Compressing completed successfully!\n");
	} else {
		decompress(argv[2], argv[3]);
		printf("Completed!\nSTATUS: Decompressing completed successfully!\n");
	}
	return 0;
}