#include <unistd.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sys/wait.h>
#include <climits>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

using namespace std;

void piping(char**, char**);
void parseCommand(string, string , vector<string>*, char** *);
char** parseCommand2 (string);
void redirect(char**);

int main() {
	int status;

	char linea[50];
	string tempLinea;
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
	bool isPipe;
	bool isRedirect;
	bool piped = false;
	vector<string> viktor;

	// char** firstCommand;
	// char** secondCommand;

	while (1) {

		isRedirect = false;
		isPipe = false;
		cout << ">";
	   	comando = NULL;
	   	param = NULL;
	   	cin.clear();
		fflush(stdin);
	   	memset(linea, 0, sizeof(linea));
	   	cin.getline(linea, 50);
	   	tempLinea = linea;

	   	/*********************************
	   	 *	INICIO PARTE DEL PIPE        *
	   	 *********************************/

	   	// cout << "linea "<<linea <<endl;

	   	if (!tempLinea.empty()){
	   		for (int i = 0; i < 50; ++i){
		   		if (linea[i]  == '|'){
		   			isPipe = true;
		   			break;
		   		}else if (linea[i] == '>'){
		   			isRedirect = true;
		   			break;
		   		}
		   	}
	   	}


	   	if (isPipe){
	   		piped =true;

	   		string cmd1, cmd2;
	   		char** firstCommand;
			char** secondCommand;
		   	cmd1 = strtok(linea, "|");
		   	cmd2 = strtok(NULL, "");
		   	// cout << "cmd1 : " << cmd1 <<endl;
		   	// cout << "cmd2 : " << cmd2 <<endl;
		   	firstCommand  = parseCommand2(cmd1);
		   	secondCommand  = parseCommand2(cmd2);
		   	// cout << "firstCommand[0]: " << firstCommand[0] << endl;
		   	// cout << "firstCommand[1]: " << firstCommand[1] << endl;
		   	// cout << "secondCommand[0]: " << secondCommand[0] << endl;
		   	// cout << "secondCommand[1]: " << secondCommand[1] << endl;
		   	piping(firstCommand, secondCommand);
		   	// cout << "SALIO DE PIPE" << endl;

		   	free(firstCommand[0]);
		   	free(firstCommand[1]);
		   	free(secondCommand[0]);
		   	free(secondCommand[1]);
		   	free(firstCommand);
		   	free(secondCommand);

		   	// cout << "presione enter" <<endl;
		   	// memset(linea, 0, sizeof(linea));
	   		// cin.getline(linea, 50);

			// string comandoCadena(linea);
		   	// string delim = "|";
		   	// parseCommand(comandoCadena, delim, &viktor, NULL);


		   	// vector<string> parsed;
		   	// vector<string> parsed2;
		   	// delim = " ";
		   	// //parseo de comandos
		   	// parseCommand(viktor.at(0), delim, &parsed, NULL);
		   	// parseCommand(viktor.at(1), delim, &parsed2, NULL);

		   	// comandos = new char[parsed.at(0).length() + 1];
		   	// strcpy(comandos, parsed.at(0).c_str());
		   	// params = new char[parsed.at(1).length() + 1];
		   	// strcpy(params, parsed.at(1).c_str());


		   	// comandos2 = new char[parsed2.at(0).length() + 1];
		   	// strcpy(comandos2, parsed2.at(0).c_str());
		   	// params2 = new char[parsed2.at(1).length() + 1];
		   	// strcpy(params2, parsed2.at(1).c_str());

		   	// cout << comandos << endl;
		   	// cout << params << endl;
		   	// cout << "-------"<<endl;
		   	// cout << comandos2 << endl;
		   	// cout << params2 << endl;

		   	//pipe

		   	// int zelda[2];
		   	// pid_t pid;
		   	// char foo[4096];

		   	// cout<<"-------------"<<endl;
		   	// if(pipe(zelda) == -1)
		   	// 	die("pipe");

		   	// if ((pid = fork()) == -1)
	    	// 	die("fork");

	    	// if(pid == 0) {
	    	// 	dup2 (zelda[1], STDOUT_FILENO);
			   //  close(zelda[0]);
			   //  close(zelda[1]);
			   //  execlp(comandos, comandos, params, (char *)0);

			   //  die("execlp");
	    	// }else{
	    	// 	close(zelda[1]);
			   //  int nbytes = read(zelda[0], foo, sizeof(foo));
			   //  printf("Output: (%.*s)\n", nbytes, foo);
			   //  wait(NULL);
			   //  execlp(comandos2, comandos2, foo, params2, (char*) 0);
		   	// }
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
	   	}else if(isRedirect){

	   		char** commandArray = new char*[4];

			char* commandRedirect;
	   		char* argumentRedirect;
	   		char* operatorType;
	   		char* fileName;

	   		commandRedirect =  strtok(linea, " ");
	   		argumentRedirect =  strtok(NULL	, " ");
	   		operatorType = strtok(NULL	, " ");

	   		if (argumentRedirect[0] == '>' || argumentRedirect[0] == '2' ){

	   			commandArray[0] = commandRedirect;
	   			commandArray[1] = argumentRedirect;
	   			commandArray[2] = operatorType;





	   		}else {
				fileName = strtok(NULL	, " ");
	   			commandArray[0] = commandRedirect;
	   			commandArray[1] = argumentRedirect;
	   			commandArray[2] = operatorType;
	   			commandArray[3] = fileName;
	   		}
	   		//inicio de la funcion redirect
	   		redirect(commandArray);





	   		//liberando espacio
	   		delete commandArray;


	   	}else{
	   		if (!tempLinea.empty()){
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
			   	} else if(strcmp(comando, "cd") == 0){
			   		if(chdir(param) < 0) {
			   			cout<< "Error cd" <<endl;
			   		}
			   	} else if ((pid = fork()) == 0) {
						if (strcmp(comando, "mi_pos") == 0) {
							std::ostringstream oss;
							comando = "cat";
							string processDirectory = "/proc/";
							string status = "/status";
							pid = getppid();
							oss << pid;
							string processId = "";
							processId += oss.str();
							processDirectory += processId;
							processDirectory += status;
							cout << processDirectory << endl;
							char processStateDirectory[processDirectory.length()];
							param = "";
							strcpy(param, processStateDirectory);
							execlp(comando, comando, param, NULL);
						} else {
							execlp(comando, comando, param, NULL);
						}
			   	}else if((pid = fork())!=0){
			   		wait(NULL);
			   	}
			}else {
			   	// cout << "error prrin" <<endl;
			}


		}



	}

	return 0;
}


void piping(char** cmd1, char** cmd2){
	int fds[2]; // file descriptors
	pipe(fds);
	pid_t pid;

	// child process #1
	if (fork() == 0) {
		// Reassign stdin to fds[0] end of pipe.
		dup2(fds[0], 0);

		// Not going to write in this child process, so we can close this end
		// of the pipe.
		close(fds[1]);

		// Execute the second command.
		execvp(cmd2[0], cmd2);
		perror("execvp failed");

		// child process #2
	} else if ((pid = fork()) == 0) {
		// Reassign stdout to fds[1] end of pipe.
		dup2(fds[1], 1);

		// Not going to read in this child process, so we can close this end
		// of the pipe.
		close(fds[0]);

		// Execute the first command.
		execvp(cmd1[0], cmd1);
		perror("execvp failed");

		// parent process
	} //else
	    // waitpid(pid, NULL, 0);
}


void redirect(char** commands){
	if (commands[3] == NULL){
		cout<< "tamalo"<<endl;
		//ejecutar redirect con el comando, operador y el archivo
	}else{
		cout<< "tabueno"<<endl;
		//ejecutar redirect con el comando, argumento, operador y el archivo
	}
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

char** parseCommand2 (string buffer) {
	vector<string> strings;
  	vector<char*> pointerChars;
  	// string buffer;
  	string temporary;
  	// getline(cin, buffer);
  	// cout << "buffer: "<< buffer << endl;
  	for (size_t i = 0; i < buffer.length() + 1; i++) {
    	if (buffer[i] != ' ') {
      	temporary += buffer[i];
	      	if (i + 1 == buffer.length()) {
	        	strings.push_back(temporary);
	        	temporary = "";
	      	}
    	} else {
      	strings.push_back(temporary);
      	temporary = "";
    	}
  	}

  	int size = strings.size();
  	char** sentences;
  	sentences = (char**) malloc(size);
  	for (size_t i = 0; i < strings.size(); i++) {
	    size = strings.at(i).length();
	    sentences[i] = (char*) malloc(size);
	    strcpy(sentences[i], strings.at(i).c_str());
  	}
  	return sentences;

  	//LIBERANDO MEMORIA
  	// for (size_t i = 0; i < strings.size(); i++) {
   //  	cout << sentences[i] << endl;
   //  	free(sentences[i]);
  	// }
  	// free(sentences);
}
