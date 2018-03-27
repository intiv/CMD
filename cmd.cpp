#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

using namespace std;

void piping(char**, char**);
void parseCommand(string, string , vector<string>*, char** *);

int main() {
	int status;

	char linea[50];
	char *comandos;
	char *comandos2;
	char *params;
	char *params2;
	char *comando;
	char *param;
	char *oper;
	char *param1;
	int pid;
	FILE *fd,*fd1;
	char c;
	vector<string> viktor;
	while (1) {

		cout << ">";
	   	comando = NULL;
	   	param = NULL;
	   	cin.clear();
		fflush(stdin);
	   	memset(linea, 0, sizeof(linea));
	   	cin.getline(linea, 50);

	   	/*********************************
	   	 *	INICIO PARTE DEL PIPE        *
	   	 *********************************/
	   	string comandoCadena(linea);
	   	string delim = "|";
	   	parseCommand(comandoCadena, delim, &viktor, NULL);


	   	vector<string> parsed;
	   	vector<string> parsed2;
	   	delim = " ";
	   	//parseo de comandos
	   	parseCommand(viktor.at(0), delim, &parsed, NULL);
	   	parseCommand(viktor.at(1), delim, &parsed2, NULL);

	   	comandos = new char[parsed.at(0).length() + 1];
	   	strcpy(comandos, parsed.at(0).c_str());
	   	params = new char[parsed.at(1).length() + 1];
	   	strcpy(params, parsed.at(1).c_str());


	   	comandos2 = new char[parsed2.at(0).length() + 1];
	   	strcpy(comandos2, parsed2.at(0).c_str());
	   	params2 = new char[parsed2.at(1).length() + 1];
	   	strcpy(params2, parsed2.at(1).c_str());

	   	// cout << comandos << endl;
	   	// cout << params << endl;
	   	// cout << "-------"<<endl;
	   	// cout << comandos2 << endl;
	   	// cout << params2 << endl;

	   	//pipe 
	   	int zelda[2];
	   	pid_t pid;
	   	char foo[4096];

	   	cout<<"-------------"<<endl;
	   	if(pipe(zelda) == -1)
	   		die("pipe");

	   	if ((pid = fork()) == -1)
    		die("fork");

    	if(pid == 0) {
    		dup2 (zelda[1], STDOUT_FILENO);
		    close(zelda[0]);
		    close(zelda[1]);
		    execlp(comandos, comandos, params, (char *)0);
		    
		    die("execlp");
    	}else{
    		close(zelda[1]);
		    int nbytes = read(zelda[0], foo, sizeof(foo));
		    printf("Output: (%.*s)\n", nbytes, foo);
		    wait(NULL);
		    execlp(comandos2, comandos2, foo, params2, (char*) 0);


    	}
    	/*********************************
	   	 *	FIN PARTE DEL PIPE           *
	   	 *********************************/
	   	
	   	// for (int i = 0; i < viktor.size(); ++i)
	   	// {
	   	// 	if(i == 0)
	   	// 		parseCommand(viktor.at(i), delim, NULL, &comandos);
	   	// 	else
	   	// 		parseCommand(viktor.at(i), delim, NULL, &comandos2);
	   	// }
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
			// execlp(comando, comando, param, NULL);
	   	}else if((pid = fork())!=0){
	   		wait(NULL);
	   	}
	}

	return 0;
}


void piping(char** firstCommand, char** secondCommand){
	int fds[2];
	pipe(fds);
	pid_t pid;
	// char** firstCommand;
	// char** secondCommand;
	// int counter = 0;
	// char* temporary;
	// for (size_t i = 0; i < strlen(commands); i++) {
	// 	if (commands[i] != '|') {
	// 		if (commands[i] == ' ') {
	// 			counter++;
	// 		} else {
	// 			temporary[0] = commands[i];
	// 			if (counter == 0) {
	// 				firstCommand[0] = appendCharToCharArray(firstCommand[0],commands[i]);
	// 			} else if (counter == 1) {
	// 				firstCommand[1] = appendCharToCharArray(firstCommand[1],commands[i]);
	// 			} else if (counter == 2) {
	// 				secondCommand[0] = appendCharToCharArray(secondCommand[0],commands[i]);
	// 			} else if (counter == 3) {
	// 				secondCommand[1] = appendCharToCharArray(secondCommand[1],commands[i]);
	// 			}
	// 		}
	// 	}
	// }
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

void parseCommand(string commandString, string delim, vector<string>* viktor, char** *arreglo) {
	size_t pos = 0;
	string token;
	char** retVal;
	int cont = 0;
	// if(delim != " "){
	   	
	   	while((pos = commandString.find(delim)) != string::npos){
	   		token = commandString.substr(0, pos);
	   		viktor->push_back(token);
	   		commandString.erase(0, pos + delim.length());
	   	}
	   	viktor->push_back(commandString);
	   	// return NULL;
   // }else{
   // 		cout << "Antes de empezar" <<endl;
   // 		while((pos = commandString.find(delim)) != string::npos){
	  //  		token = commandString.substr(0, pos);
	  //  		// *(arreglo[cont]) = (char*) alloca(token.length() + 1);
	  //  		// memcpy(*(arreglo[cont]), token.c_str(), token.size() + 1);
	  //  		*(arreglo[cont]) = new char[token.length() + 1];
	  //  		strcpy(*(arreglo[cont]), token.c_str());
	  //  		cout << "Prueba" <<endl;
	  //  		cont++;
	  //  		commandString.erase(0, pos + delim.length());
	  //  	}
	  //  	cout << "Antes del ultimo"<<endl;
	  //  	// *(arreglo[cont]) = (char*) alloca(commandString.size() + 1);
	  //  	// memcpy(*(arreglo[cont]), commandString.c_str(), commandString.size() + 1);
	  //  	// return &retVal;
   // }
}