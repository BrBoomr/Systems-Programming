#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//TODO -
	//Implement Functions[x]
	//Implement Exit loop in main function[x]
	//DRY it up.[]
	
//User Input, Command, Param1, Param2, Length of User Input
void breakUp(char *, char *, char*, char*, int );
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
	//user_input holds the long command-line string. Will be broken up amongst the following variables.
	
	int terminate=0;
	while(terminate != 1){
		char user_input[80];
		char c[10];
		char p1[10];
		char p2[10];
		
		//Determines the length of the command.
		int c_length;
		//Determines if user has typed 'exit' as a command. If so, it should return 1 and thus break the following loop.
		
		//While the interpret function does not return 1 (called exit command).

		printf("Command: ");
		
		//Read user input
		fgets(user_input, 80, stdin);

		//Get rid of character newline '\n'
		c_length = strlen(user_input) - 1;
		if(user_input[c_length] == '\n'){
			user_input[c_length] = '\0';
		}
		
//TESTING TOOL
//printf("Length: %d\n",c_length);
		printf("VARIABLES\n");
		for(int x=0; x<10; x++){
			printf("%c", c[x]);
		}
		printf("\n");
		for(int x=0; x<10; x++){
			printf("%c", p1[x]);
		}
		printf("\n");
		for(int x=0; x<10; x++){
			printf("%c", p2[x]);
		}
		printf("\n");
		
		
		//Break the user_input apart into different variables.
		breakUp(user_input, c, p1, p2, c_length);
		
		//After command has been brokenUp, interpret the command and its paramaters.
		//Interpret function will return a value after it has been called, determining the fate of the loop.
		terminate=interpreter(c,p1,p2);
		
//TESTING TOOL
printf("Command: %s\n", c);
printf("Param1: %s\n", p1);
//printf("Param2: %s\n", p2);


	}

}
/*
	breakUp function will contain a while-loop that iterates until a variable, counter, reaches the max-amount of characters found in the user-input.
	This ensures that the arrays do not suffer from overflow.
	Each iteration of the while-loop will assign a char to the char-array variables c[],p1[],p2[].
	When user_input[] reaches a whitespace, it will NOT assign it but instead move up a paramater. c->p1->p2.

*/
void breakUp(char *user_input, char *c, char *p1, char *p2, int c_length){

	c[0] = 0; p1[0]=0; p2[0]=0;
	//Variable used as an index counter to user_input amongst the command-line parameters. Continous; value remains while index resets.
	int counter=0;
	//Delimeter; determines the stage of the loop in which of the parameters in the command-line will be initialized.
	int delim=0;
	//Index for the parameters. Discontinuous, resets every new delimeter.
	int index=0;


	//We shall run this loop until the counter reaches the end of the command-line, in which case it should have already assigned all char arrays.
	while(counter!=c_length){
		//If the command-line reaches a whitespace, it is a new parameter. Moves from Command->Param1->Param2
		if(user_input[counter]== ' '){
			delim++;
			index=0;
		}

		//Command Assignment
		else if(delim==0){
			c[index]=user_input[counter];
//TESTING TOOL
//printf("c[index]=%c\n",c[index]);
		}

		//Parameter 1 Assignment
		else if(delim==1){
			p1[index]=user_input[counter];
//TESTING TOOL
//printf("p1[index]=%c\n", p1[index]);
		}

		//Paramter 2 Assignment
		else if(delim==2){
			p2[index]=user_input[counter];
//TESTING TOOL
//printf("p2[index]=%c\n", p2[index]);
		}
	index++;
	counter++;
	}
//TESTING TOOL
//printf("Counter: %d\n", counter);
}

/*
	interpreter will have a list of comparisons based on the given command prompts.
	If c[] matches any of these, we will call their respective function and send appropriate paramaters.
	If any paramaters have been initialized for commands that do not take them, their respective function will catch them and inform the user of error. Vise-Versa.

*/

int interpreter(char *c, char *p1, char *p2){
	
	//Holds value for the comparison operation.
	int cmp;
	//We will go through the whole list of available commands and evaluate if the c[] array matches. If not, error prompt will display.
	//If matched, we call the appropriate functions and send in the paramaters if any apply.
	cmp = strcmp(c,"load");
	if(cmp == 0){
		if(p1[0]=='\0'){
		printf("load command is missing 1/1 commands.\n Use command 'help' to list commands available.\n");
		}
		else{
			printf("Command 'load' is a WIP.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c,"execute");
	if(cmp == 0){
		if(p1[0]=='\0'){
		printf("Command 'execute' is a WIP.\n");
	}
		else{
			printf("execute command does not take parameters.\n Use command 'help' to list available commands.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c,"debug");
	if(cmp==0){
		if(p1[0]=='\0'){
		printf("Command 'debug' is a WIP.\n");
		}
		else{
			printf("debug command does not take paramenters.\n Use command 'help' to list available commands.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c, "dump");
	if(cmp==0){
		if(p1[0]=='\0' && p2[0]=='\0'){
		printf("dump command is missing 2/2 commands.\n Use command 'help' to list commands available.\n");
		}
		else if(p2[0]=='\0'){
			printf("dump command is missing 1/2 commands.\n Use command 'help' to list commands available.\n");
		}
		else{
			printf("Command 'dump' is a WIP.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c, "help");
	if(cmp==0){
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
		return 0;
	}
	
	cmp = strcmp(c, "assemble");
	if(cmp==0){
		if(p1[0]=='\0'){
		printf("assemble command is missing 1/1 commands.\n Use command 'help' to list commands available.\n");
		}
		else{
		printf("Command 'assemble' is a WIP.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c, "directory");
	if(cmp==0){
		if(p1[0]=='\0'){
		printf("Showing Directory\n\n");
		system("ls\n");
		}
		else{
			printf("directory command does not take paramaters.\n Use command 'help' to list commands available.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c, "exit");
	if(cmp==0){
		if(p1[0]=='\0'){
		printf("Exiting Program...\n");
		}
		else{
			printf("exit command does not take paramaters.\n Use command 'help' to list commands available.\n");
		}
		return 1;
	}
	
	//Error prompt after all conditions have failed.
	printf("Invalid Input.\n Use command 'help' to list commands available.\n");
}
