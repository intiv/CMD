#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


using namespace std;

void piping(char*, char*, int);



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
	   			comandos = strtok(linea, "|");
	   			comandos2 = strtok(NULL, "\n");
	   			cout << comandos << endl;
	   			piping(comandos, comandos2, 2);
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


void piping(char* commands1, char* commands2, int size){
	int fds[2];
	pipe(fds);
	pid_t pid;
	char** c1;
	char** c2;
	cout << "Antes de commands" <<endl;
	c1[0] = strtok(commands1, " ");
	cout <<"C1:0 - "<< c1[0] << endl;
	c1[1] = strtok(NULL, " ");
	cout <<"C1:1 - "<< c1[1] << endl;

	cout << commands2 << endl;
	// commands2 = commands2 + 1;
	c2[0] = strtok(commands2, " ");
	if(c2[0]){	
		cout <<"C2:0 - "<< c2[0] << endl;
	}else{
		cout<<"Neles"<<endl;
	}
	c2[1] = strtok(NULL, " ");
	cout <<"C2:1 - "<< c2[1] << endl;
	
	if (fork() == 0) {
    // Reassign stdin to fds[0] end of pipe.
	    dup2(fds[0], 0);

	    // Not going to write in this child process, so we can close this end
	    // of the pipe.
	    close(fds[1]);

	    // Execute the second command.
	    execvp(c2[0], c2);
	    perror("execlp failed");

  // child process #2
  	} else if ((pid = fork()) == 0) {
    // Reassign stdout to fds[1] end of pipe.
	    dup2(fds[1], 1);

	    // Not going to read in this child process, so we can close this end
	    // of the pipe.
	    close(fds[0]);

	    // Execute the first command.
	    execvp(c1[0], c1);
	    perror("execlp failed");

  // parent process
  } else
    waitpid(pid, NULL, 0);

}
