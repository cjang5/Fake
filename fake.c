#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    //for fork
#include <sys/types.h> //for waitpid
#include <sys/wait.h>  //for waitpid

//main method
int main(int argc, char* argv[]) {
	/**
	 * first we scan the arguments in argv to get the name
	 * of the target we want to compile in the Fakefile
	 * It will be in argv[1], and if not, we throw an error
	 */
	if (argv[1] == 0) {
		puts("[FAKE]: ERROR - No argument!");
		return 1;
	} 

	/**
	 * store the argv[1] as the target we will look for in Fakefile
	 * we will also append a colon : for easier scanning in Fakefile
	 */
	char * target = argv[1];
	target = strcat(target, ":");

	/**
	 * Otherwise, we first check that the Fakefile exists in the current
	 * directory. 
	 * If it does, we proceed, if not, we throw an error
	 */
	FILE * Fakefile = fopen("Fakefile", "r");

	//if there is no Fakefile
	if (Fakefile == NULL) {
		puts("[FAKE]: ERROR - No Fakefile found!");
		return 1;
	}

	/**
	 * At this point, we know there is a Fakefile in the current
	 * directory, and that we have a target argument to work with
	 * So we scan through the Fakefile until EOF (End of File)
	 * and if we find the target, good!
	 * Otherwise, throw an error
	 */
	char * currLine = NULL;
	size_t currLineSize = 0;
	int found = 0; // 0 = not found, 1 = found
	while (getline(&currLine, &currLineSize, Fakefile) > -1 && found == 0) {
		//search for 'target' in the current line
		char * foundTarget = strstr(currLine, target);

		//If the 'target' is found, we flip 'found' to 1 (true)
		if (foundTarget != NULL)
			found = 1;
	}

	//If we didn't find 'target' (found == 0) then we throw an error
	if (found == 0) {
		printf("[FAKE]: ERROR - Target: \'%s\' not found!\n", target);
		return 1;
	} else {
		if (strcmp(target, "clean:"))
			printf("[FAKE]: Found target \'%s\'\n", target);
	}

	//make clean
	/*
	if (!strcmp(target, "clean:")) {
		//create the backup directory
		int status;
		pid_t child = fork();

		if (child == -1) {
			printf("[CLEAN]: Failed to fork\n");
			return 1;
		} else if (child > 0) {
			pid_t pid = waitpid(child, &status, 0);

			if (WIFSIGNALED(status)) { //if child ends due to signal
				printf("[FAKE]: Child exited due to signal %d\n", WTERMSIG(status));

				return 1;
			}
		}
		//child
		else {
			//create the backup directory
			execlp("bash", "bash", "-c", "mkdir backup", (char *) NULL);
			printf("mkdir failed!\n");
		}

		//Now we move the files we want to keep into the backup directory
		do {
			if (currLine[0] == '\n' || strlen(currLine) == 0) 
				return 0;
			else if (currLine[0] != '\t') 
				return 1;
				
			char * command = currLine + 1;

			if (command[strlen(command) - 1] == '\n')
				command[strlen(command) - 1] = 0;

			child = fork();
			
			if (child == -1) {
				printf("[CLEAN]: Failed to fork\n");
				return 1;
			} else if (child > 0) {
				pid_t pid = waitpid(child, &status, 0);

				if (WIFSIGNALED(status)) { //if child ends due to signal
					printf("[FAKE]: Child exited due to signal %d\n", WTERMSIG(status));

					return 1;
				}
			}
			//child
			else {
				//create the backup directory
				execlp("bash", "bash", "-c", "mv %s backup", command);
				printf("mkdir failed!\n");
			}

		} while (getline(&currLine, &currLineSize, Fakefile) > -1);
	} */
		
    /**
     * We will get each line after finding the command and make sure
     * it starts with a \t otherwise it isn't valid syntax
     * We will 
     */
	do {
		if (currLine[0] == '\n' || strlen(currLine) == 0) 
			return 0;
		else if (currLine[0] != '\t') {
			printf("[FAKE]: ERROR - Not a valid command!\n");
			return 1;
		}

		//new char* to store commands
		char * command = currLine + 1;

		/**
		 * Get rid of \n if it is the last char in 'command'
		 * replace it with a nullbyte
		 */
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = 0;

		/**
		 * print what command we are about to execute
		 * as long as the target isn't clean
		 */
		if (strcmp(target, "clean:"))
			printf("[FAKE]: Running \'%s\'\n", command);

		/**
		 * We fork here and the child runs the next command
		 * The parent will -- in the meantime -- wait for the child 
		 * process to finish
		 */
		int status;
		pid_t child = fork();

		//error check first to potentially save time
		if (child == -1) {
			perror("[FAKE]: ERROR - Fork failed\n");
			return 1;
		} 
		//otherwise, in the parent
		else if (child > 0) {
			//We wait for the child process to finish
			pid_t pid = waitpid(child, &status, 0);

			/**
			 * Then we print to the console based on what return
			 * status the child process ended with
			 */
			if (WIFSIGNALED(status)) { //if child ends due to signal
				printf("[FAKE]: Child exited due to signal %d\n", WTERMSIG(status));

				//for specific signals
				if (WTERMSIG(status) == SIGSEGV)
					printf("[FAKE]:     Signal {%d}: Segmentation Fault\n", WTERMSIG(status));
				//maybe add more

				//return with exit code 1
				return 1;
			}
		}
		//otherwise, we are in the child
		else if (child == 0) {
			//if make clean is called, clean up the folder
			if (!strcmp(target, "clean:")) {
				//printf("CLEAN!\n");
				//return 0;
			} else {
				//let exec run the command in bash
				execlp("bash", "bash", "-c", command, (char *) NULL);
				printf("EXEC ERROR\n"); //if this runs then exec failed
			}			
		}
	} while(getline(&currLine, &currLineSize, Fakefile));

	fclose(Fakefile);
	
	return 0;
}















