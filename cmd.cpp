#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main() { 
	int status;
	char linea[20];
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
	   	memset(linea, 0, sizeof(linea));
	   	cin.getline(linea, 50);
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
	   		cout << "Case cd" << endl;
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
