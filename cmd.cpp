#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
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
char getcharacter();
void autocompletar(string*);

int main() {
	int status;
	string linea1;
	char* linea;
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
	bool emptyline = true;
	while (1) {

		isRedirect = false;
		isPipe = false;
	   	comando = NULL;
	   	param = NULL;
	   	linea1 = "";

		cout << ">";
		cin.clear();
		fflush(stdin);

		if(!emptyline){
			memset(linea, 0, sizeof(linea));
		}
		char t = 0;
		bool autocomplete = false;
		while(true){
			t = getcharacter();
			if(t == '\t'){
				autocomplete = true;

				break;
			}else if(t == '\n'){
				autocomplete = false;
				break;

			}else{
				linea1 += t;
			}
		}

		cout << endl;
		if(autocomplete){
			autocompletar(&linea1);
			cout << ">"<<linea1;
			t = 0;
			while(true){
				t = getcharacter();
				if(t == '\n'){
					break;
				}else{
					linea1 += t;
				}
			}
		}
	   	// cin.getline(linea, 50);
	   	linea = const_cast<char*>(linea1.c_str());
	   	emptyline = false;
	   	tempLinea = linea;
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
		   	firstCommand  = parseCommand2(cmd1);
		   	secondCommand  = parseCommand2(cmd2);
		   	piping(firstCommand, secondCommand);

		   	free(firstCommand[0]);
		   	free(firstCommand[1]);
		   	free(secondCommand[0]);
		   	free(secondCommand[1]);
		   	free(firstCommand);
		   	free(secondCommand);
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
							int pid2 = getppid();
							oss << pid2;
							string processId = "";
							processId += oss.str();
							processDirectory += processId;
							processDirectory += status;
							param = const_cast<char*>(processDirectory.c_str());
							execlp(comando, comando, param, NULL);
						} else {
							
							execlp(comando, comando, param, NULL);
						}
			   	}else if((pid = fork())!=0){
			   		wait(NULL);
			   	}
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
	} 
}


void redirect(char** commands){	
	if (commands[3] == NULL){
		char** command = new char*[1];
		command[0] = strdup(commands[0]);
		int cual;
		if(!ifstream(commands[2])){
			ofstream create(commands[2]);
			create.close();
		}
		if(strcmp(commands[1], ">") == 0){

			cual = 0;
		}else if(strcmp(commands[1], ">>") == 0){
			cual = 1;
		}else if(strcmp(commands[1], ">&2") == 0){
			cual = 2;

		}else if(strcmp(commands[1], "2>&1") == 0){
			cual = 3;
		}
		int fds[2];
		pipe(fds);
		char c;
		int count;
		pid_t pid;
		char foo[4096];
		if(fork() == 0){
			dup2(fds[0], 0);
			close(fds[1]);
			int nbytes = read(fds[0], foo, sizeof(foo));
    		ofstream escribir;
    		if(cual == 0){
    			escribir.open(commands[2], fstream::trunc);

    		}else if(cual == 1){
    			escribir.open(commands[2], fstream::app);

    		}else if(cual == 2){
    			escribir.open(commands[2], fstream::app);

    		}else if(cual == 3){
    			escribir.open(commands[2], fstream::app);

    		}
    		escribir << foo;
    		escribir.close();
    		execlp("echo", "echo", NULL);
    		execlp("rm", "askdhad", NULL);
		}else if((pid = fork()) == 0){
			dup2(fds[1], 1);
			close(fds[0]);
			execvp(command[0], command);
		}else{
			waitpid(pid, NULL, 0);
			close(fds[0]);
			close(fds[1]);
			// fclose(archivo);
		}
		delete command;
	}else{
		char** command = new char*[2];
		command[0] = strdup(commands[0]);
		command[1] = strdup(commands[1]);

		int cual;
		if(!ifstream(commands[3])){
			ofstream create(commands[3]);
			create.close();
		}
		if(strcmp(commands[2], ">") == 0){

			cual = 0;
		}else if(strcmp(commands[2], ">>") == 0){
			cual = 1;
		}else if(strcmp(commands[2], ">&2") == 0){
			cual = 2;

		}else if(strcmp(commands[2], "2>&1") == 0){
			cual = 3;
		}
		int fds[2];
		pipe(fds);
		char c;
		int count;
		pid_t pid;
		char foo[4096];
		if(fork() == 0){
			dup2(fds[0], 0);
			close(fds[1]);
			int nbytes = read(fds[0], foo, sizeof(foo));
    		ofstream escribir;
    		if(cual == 0){
    			escribir.open(commands[3], fstream::trunc);

    		}else if(cual == 1){
    			escribir.open(commands[3], fstream::app);

    		}else if(cual == 2){
    			escribir.open(commands[3], fstream::app);

    		}else if(cual == 3){
    			escribir.open(commands[3], fstream::app);

    		}
    		escribir << foo;
    		escribir.close();
    		execlp("echo", "echo", NULL);
		}else if((pid = fork()) == 0){
			dup2(fds[1], 1);
			close(fds[0]);
			execvp(command[0], command);
		}else{
			waitpid(pid, NULL, 0);
			close(fds[0]);
			close(fds[1]);
			// fclose(archivo);
		}
		delete command;
		//ejecutar redirect con el comando, argumento, operador y el archivo
	}
}




void parseCommand(string commandString, string delim, vector<string>* viktor, char** *arreglo) {
	size_t pos = 0;
	string token;
	char** retVal;
	int cont = 0;
   	while((pos = commandString.find(delim)) != string::npos){
   		token = commandString.substr(0, pos);
   		viktor->push_back(token);
   		commandString.erase(0, pos + delim.length());
   	}
   	viktor->push_back(commandString);

}

char** parseCommand2 (string buffer) {
	vector<string> strings;
  	vector<char*> pointerChars;
  	string temporary;

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
}


char getcharacter(){
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    if(buf != '\t')
    	printf("%c",buf);
    return buf;
 }

 void autocompletar(string* line){
 	string lineaF;
 	int lsize = line->size();
 	ifstream comandos("comandos.txt");
 	vector<string> validCommands;
 	if(comandos){
 		getline(comandos, lineaF);
 		while(!comandos.eof()){
 			if(lineaF.size() >= lsize){
	 			if(lineaF.substr(0, lsize) == *line){
	 				validCommands.push_back(lineaF);
	 			}
 			}
 			getline(comandos, lineaF);
 		}
 		if(validCommands.size() == 2 && validCommands.at(0) == validCommands.at(1)){
 			cout << "line = "<<validCommands.at(0)<<endl;
 			*line = validCommands.at(0);
 		}else if(validCommands.size() > 0){
 			cout << validCommands.at(0);
 			for (int i = 1; i < validCommands.size(); ++i){
 				cout <<"  "<< validCommands.at(i);
 			}
 			cout << endl;
 		}else{ //0 comandos
 			*line = "";
 			cout << "No hay un comando al cual autocompletar a partir de " << *line <<endl;
 		}
 		comandos.close();
 	}else{
 		*line = "";
 		cout << "Error obteniendo lista de comandos"<<endl;
 	}
 }