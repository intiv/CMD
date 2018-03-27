#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


using namespace std;

void piping(char*);
char* appendCharToCharArray(char*, char);


int main() {
	int status;

	char linea[50];
	char *comandos;
	char *comandos2;
	char *comando;
	char *param;
	char *oper;
	char *param1;
	int pid;
	FILE *fd,*fd1;
	char c;

	while (1) {

		cout << ">";
	   	comando = NULL;
	   	param = NULL;
	   	cin.clear();
		fflush(stdin);
	   	memset(linea, 0, sizeof(linea));
	   	cin.getline(linea, 50);
	   	for (int i = 0; i < 50; ++i){
	   		if(linea[i] == '|'){
	   			// comandos = strtok(linea, "|");
	   			// comandos2 = strtok(NULL, "\n");
	   			// cout << comandos << endl;
	   			piping(linea);
	   		}
	   	}
	   	comando = strtok(linea," ");
	   	param = strtok(NULL," ");
	   	oper = strtok(NULL, " ");
	   	if (oper !=NULL) {
			if (strcmp(oper,">>") == 0) {
				param1 = strtok(NULL," ");
				fd = fopen(param,"r");
				fd1 = fopen(param1,"a");
				int i=0;
  				while (1) {
    				c = (char)fgetc(fd);
			  		if (c != EOF) {
						fputc(c,fd1);
			  		} else
						break;
				}
				fclose(fd);
				fclose(fd1);
			}
	   	}else if(strcmp(comando, "cd") == 0){
	   		if(chdir(param) < 0) {
	   			cout<< "Error cd" <<endl;
	   		}
	   	}else if ((pid = fork()) == 0) {
			execlp(comando, comando, param, NULL);
	   	}else if((pid = fork())!=0){
	   		wait(NULL);
	   	}
	}

	return 0;
}


void piping(char* commands){
	int fds[2];
	pipe(fds);
	pid_t pid;
	char** firstCommand;
	char** secondCommand;
	int counter = 0;
	char* temporary;
	for (size_t i = 0; i < strlen(commands); i++) {
		if (commands[i] != '|') {
			if (commands[i] == ' ') {
				counter++;
			} else {
				temporary[0] = commands[i];
				if (counter == 0) {
					firstCommand[0] = appendCharToCharArray(firstCommand[0],commands[i]);
				} else if (counter == 1) {
					firstCommand[1] = appendCharToCharArray(firstCommand[1],commands[i]);
				} else if (counter == 2) {
					secondCommand[0] = appendCharToCharArray(secondCommand[0],commands[i]);
				} else if (counter == 3) {
					secondCommand[1] = appendCharToCharArray(secondCommand[1],commands[i]);
				}
			}
		}
	}
	// c1[0] = strsep(&commands, " ");
	// c1[1] = strsep(&commands, " ");
	// cout <<"C1:0 - "<< c1[0] << endl;
	// cout <<"C1:1 - "<< c1[1] << endl;
	// strsep(&commands, "|");
	// cout << commands << endl;
	// cout << "Parse | "<<endl;
	// c1[2] = strsep(&commands, " ");
	// cout <<"C1:2 - "<< c1[2] << endl;
	//
	// c1[3] = strsep(&commands, " ");
	// cout <<"C1:3 - "<< c1[3] << endl;
	//
	// // size_t destination_size = sizeof (c1[0]);
	// strcpy(c2[0], c1[0]);
	// strcpy(c2[1], c1[1]);
	// strcpy(c3[0], c1[2]);
	// strcpy(c3[1], c1[3]);
	// cout << "---------" <<endl;
	// cout << c2[0] << endl;
	// cout << c2[1] << endl;
	// cout << c3[0] << endl;
	// cout << c3[1] << endl;

	if (fork() == 0) {
    // Reassign stdin to fds[0] end of pipe.
	    dup2(fds[0], 0);

	    // Not going to write in this child process, so we can close this end
	    // of the pipe.
	    close(fds[1]);

	    // Execute the second command.
	    execvp(secondCommand[0], secondCommand);
	    perror("execlp failed");

  // child process #2
  	} else if ((pid = fork()) == 0) {
    // Reassign stdout to fds[1] end of pipe.
	    dup2(fds[1], 1);

	    // Not going to read in this child process, so we can close this end
	    // of the pipe.
	    close(fds[0]);

	    // Execute the first command.
	    execvp(firstCommand[0], firstCommand);
	    perror("execlp failed");

  // parent process
  } else
    waitpid(pid, NULL, 0);

}

char* appendCharToCharArray(char* array, char a) {
	size_t len = strlen(array);
	char* ret = new char[len+2];
	strcpy(ret, array);
	ret[len] = a;
	ret[len+1] = '\0';
	return ret;
}
