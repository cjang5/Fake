#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]) {
	//Make sure there are actually arguments
	if (argc == 1) {
		printf("\n");
		return 0;
	}

	/**
	 * First we will try to open a file
	 * If it doesn't exist, we will just throw and error and return
	 */
	FILE * file;

	//try to open the file
	file = fopen(argv[1], "r+");

	//if the file doesnt exist
	if (file == NULL) {
		printf("Could not find file with name \'%s\'!\n", argv[1]);
		return 1;
	} else {
		//print the contents
		char * currLine = NULL;
		size_t currLineSize = 0;
		while (getline(&currLine, &currLineSize, file) != -1)
			printf("%s", currLine);
		printf("\n");
	}

	return 0;
}