#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void breakUp(char *, char *, char*, char*);
int interpreter(char *, char *, char *);

void f_load(char *);
void f_execute();
void f_debug();
void f_dump(char *, char *);
void f_help();
void f_assemble(char *);
void f_directory();
void f_exit();

void main(){
	char input[80];
	char c[10];
	char p1[10];
	char p2[10];

	int length;
	int terminate=0;

	while(terminate==0){
	printf("Enter a Command: ");
	fgets(input, 80, stdin);

	length = strlen(input) - 1;
	if(input[length]=='\n'){
		input[length]='\0';
	}

	printf("Command: %s\n", input);
	printf("Length: %d\n", length);
	
	
		breakUp(input, c, p1, p2);
		terminate=interpreter(c, p1, p2);
	}
}

void breakUp(char *input, char *c, char *p1, char *p2){
	for(int x=0; x<=10; x++){c[x]=p1[x]=p2[x] = '\0';}
	printf("------------------------------------------------\n");
	printf("Command: %s\n", input);
	int length = strlen(input)+1;
	//printf("Length: %d\n", length);
	//printf("Last Index of Command: %c\n", input[length]);

	int counter=0;
	int index=0;
	int delim=0;
	
	while(counter<length) {

		if(input[counter] == ' '){
			delim+=1;
			index=0;
		}

		if(delim==0){
			//printf(0);
			c[index] = input[counter];
			index+=1; counter+=1;
		}
		if(delim==1){
			//printf(1);
			p1[index] = input[counter];
			index+=1; counter+=1;
		}
		if(delim==2){
			//printf(2);
			p2[index] = input[counter];
			index+=1; counter+=1;
		}
	}

	//printf("Counter: %d\n", counter);
	//printf("Index: %d\n", index);
	//printf("Delim: %d\n", delim);
	//printf("Command: %s\n", c);
	//printf("Param1: %s\n", p1);
	//printf("Param2: %s\n", p2);
}

int interpreter(char *c, char *p1, char *p2){
	//printf("%s\n", c);
	//printf("%s\n", p1);
	//printf("%s\n", p2);
	//Holds value for the comparison operation.
	int cmp;
	//We will go through the whole list of available commands and evaluate if the c[] array matches. If not, error prompt will display.
	//If matched, we call the appropriate functions and send in the paramaters if any apply.
	cmp = strcmp(c,"load");
	if(cmp == 0){
		f_load(p1);
		return 0;
	}
	
	cmp = strcmp(c,"execute");
	if(cmp == 0){
		f_execute(p1);
		return 0;
	}
	
	cmp = strcmp(c,"debug");
	if(cmp==0){
		f_debug(p1);
		return 0;
	}
	
	cmp = strcmp(c, "dump");
	if(cmp==0){
		f_dump(p1, p2);
		return 0;
	}
	
	cmp = strcmp(c, "help");
	if(cmp==0){
		f_help(p1);
		return 0;
	}
	
	cmp = strcmp(c, "assemble");
	if(cmp==0){
		f_assemble(p1);
		return 0;
	}
	
	cmp = strcmp(c, "directory");
	if(cmp==0){
		f_directory(p1);
		return 0;
	}
	
	cmp = strcmp(c, "exit");
	if(cmp==0){
		f_exit(p1);
		return 1;
	}
	
	//Error prompt after all conditions have failed.
	printf("Invalid Input.\n Use command 'help' to list commands available.\n");
}

//MENU FUNCTIONS.
// load <filename> | debug | execute | dump <start> <end> | help | assemble <filename> | directory | exit 
// Functions will check for the status of parameters. If something exists within the array, it was initialized.
// Functions who take parameters will approve while others will reject the usage of parameter and prompt to try again. Vise-Versa.
void f_load(char *p1){
	if(p1[0]=='\0'){
		printf("load command is missing 1/1 commands.\n Use command 'help' to list commands available.\n");
	}
	else{
		printf("Command 'load' is a WIP.\n");
	}
}
void f_execute(char *p1){
	if(p1[0]=='\0'){
		printf("Command 'execute' is a WIP.\n");
	}
	else{
		printf("execute command does not take parameters.\n Use command 'help' to list available commands.\n");
	}
}
void f_debug(char *p1){
	printf("Command 'debug' is a WIP.\n");
}
void f_dump(char *p1, char *p2){
	if(p1[0]=='\0' && p2[0]=='\0'){
		printf("dump command is missing 2/2 commands.\n Use command 'help' to list commands available.\n");
	}
	else if(p2[0]=='\0'){
		printf("dump command is missing 1/2 commands.\n Use command 'help' to list commands available.\n");
	}
	else{
		printf("Command 'dump' is a WIP.\n");
	}
}
void f_help(char *p1){
	if(p1[0]=='\0'){
		printf("load <filename>\n");
		printf("execute\n");
		printf("debug\n");
		printf("dump start end\n");
		printf("help\n");
		printf("assemble <filename>\n");
		printf("directory\n");
		printf("exit\n");
	}
	else{
		printf("help command does not take parameters.\n use command 'help' to list available commands.\n");
	}
}
void f_assemble(char *p1){
	if(p1[0]=='\0'){
		printf("assemble command is missing 1/1 commands.\n Use command 'help' to list commands available.\n");
	}
	printf("Command 'assemble' is a WIP.\n");
}
void f_directory(char *p1){
	if(p1[0]=='\0'){
		printf("Showing Directory\n\n");
		system("ls\n");
	}
	else{
		printf("directory command does not take paramaters.\n Use command 'help' to list commands available.\n");
	}
}
void f_exit(char *p1){
	if(p1[0]=='\0'){
		printf("Exiting Program...\n");
	}
	else{
		printf("exit command does not take paramaters.\n Use command 'help' to list commands available.\n");
	}
}
