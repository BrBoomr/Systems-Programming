//Jafet Reyes
//CSCI 3334

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
//PHASE 1 FUNCTION PROTOTYPES
void breakUp(char *, char *, char*, char*);
void breakUp2(char *, char *, char*, char*);
int interpreter(char *, char *, char *);

//PHASE 2 FUNCTION PROTOTYPES
	//Main-Menu Interface(only moving algorithms to function calls).
void load(char*);
void execute();
void debug();
void dump(char*, char *);
void help();
void assemble(char*); //Modified for Phase 2.
void directory();

	//Phase 2 Functions
//Professor recommended using a struct to hold the Intermediate File items.
struct I_Item
{
	int Location;
	int OpCode;
	char Operand[10];
	int Error;
	int Line;
	struct I_Item *next;
};
struct I_Item *root = NULL;
struct I_Item *current = NULL;

//Phase 2 will use a hash table for the OpCode

struct H_Item {
	char state[10];
	int OpCode;
	struct H_Item *next;
};
struct H_Item *H_root = NULL;
struct H_Item *H_current = NULL;
struct H_Item* H_Table[100];

struct S_Item {
	char state[10];
	int address;
	struct S_Item *next;
};
struct S_Item *S_root = NULL;
struct S_Item *S_current = NULL;
struct S_Item* S_Table[100];

//With student guidance, I decided to use a key-based search instead.
//HASH TABLE FUNCTIONS----------------------------------

//Creates the Hash Table
void Create_H_Table();
//Adds an item to Hash Table
void Add_H_Item(char *, int);
//Displays the Hash Table
void Show_H_Table();
//Counts items based on the index provided.
int Count_H_Items(int);
//Displays items based on the index provided.
void Show_H_Items(int);
//Creates the OpCode Table
void OpCodeTable();
//Searches the table, returns a non-0 number if found.
int Search_H_Table(char *);
//Generates the key aforementioned.
int HashKey(char *);
//----------------------------------------------------

//SYMBOL TABLE FUNCTIONS------------------------------

//Create the Symbol Table
void Create_S_Table();
//Adds an item to Symbol Table
void Add_S_Item(char*, int);
//Displays the Symbol Table
void Show_S_Table();
//Displays items based on the index provided.
void Show_S_Items(int);
//Search the Symbol Table, returns a non-0 integer if found
int Search_S_Table(char *);
//Writes to symFile.txt
void toSymFile();

//-----------------------------------------------------


//A better version of breakUp. Used for the testing file.
void newBreakUp(char *, char *, char *, char *);

//Write to intermediate file.
void toIntermFile();

//Read from intermediate file.
//Location | OpCode | Operand | Error | Line
void interpretIntermFile(int, int, char *, int, int);


void main(){
	//Input will hold the command-line inputted by user.
	char input[80];
	//Input is broken down in 3 parts and assigned accordingly in BreakUp();
	char c[10];
	char p1[10];
	char p2[10];
	//Holds the length of the command-line.
	int length;
	//Variable used to break out of the program.
	//Wil evaluate to 1 if user enters 'exit' as a command.->
	int terminate=0;
	
	//->Any other command will return 0.
	while(terminate==0){
	printf("Enter a Command: ");
	fgets(input, 80, stdin);
	
	//We eliminate the trailing newline out of the command-line.
	length = strlen(input) - 1;
	if(input[length]=='\n'){
		input[length]='\0';
	}

	//printf("Command: %s\n", input);
	//printf("Length: %d\n", length);
	
		//We call the breakup function and send in the command-line and the variables
		//...used to store it in seperate parts.
		breakUp2(input, c, p1, p2);
		//After c[], p1[], p2[] hold the command-line parts, we send them over to be
		//...interpreted. The function returns a value, thus we assign it to terminate.
		terminate=interpreter(c, p1, p2);
	}
}

/*
breakUp function will break the command-line (input) into the according variables.
The first char-array will be considered the command, and the subsequent variables will be its params.
We use the same variable names to be consistent and use less brainpower.

*/
void breakUp(char *input, char *c, char *p1, char *p2){
	//We initialize the command and params arrays for every index to be equals to null.
	//This ensures a clean-array next iteration of entering commands.
	//SAMPLE: help -> directory -> helpctory

	//Either of these will reset the arrays to contain nothing, however the for-loop worked before the latter did, so I'm sticking to it.
	for(int x=0; x<=10; x++){c[x]=p1[x]=p2[x] = '\0';}
	//c[0]=p1[0]=p2[0]='\0';

	//Formatting.
	printf("------------------------------------------------\n");
	printf("Command: %s\n", input);
	
	//When we remove the newline from the original input char array, we remove an index.
	//..We give it back to ensure the following loops imports all the proper characters.
	int length = strlen(input);

	//printf("Length: %d\n", length);
	//printf("Last Index of Command: %c\n", input[length]);
	
	//Counter is used as an index for input. It is a continous variable that is incremented
	//..each iteration. When it reaches the maximum length of the command-line, we break out.
	//...since it should have already finished assigning chars to the arrays.
	int counter=0;
	//Index is the index for the arrays that will store the broken command-line. 
	//..it is reset after a whitespace is reached, since the next char array will be used from
	//...index=0; c[9] -> p1[0]
	int index=0;
	//Delim will determine the stage of the loop in which variable is going to be assigned
	//..chars to. Alongside index, it is incremented every whitespace but it is continuous.
	//...its max value should be 3, since there is only 3 parts to the command-line.
	int delim=0;
	
	//If counter reaches the length of the command-line, breakout.
	while(counter<length) {
		//If the command-line[counter] is a whitespace, reset index and move to the next arr.
		if(input[counter] == ' '){
			delim+=1;
			index=0;
		}
		//DELIM VALUES:
		//0=c[], 1=p1[], 2=p2[]
		//------------------------------
		if(delim==0){
			//printf(0);
			
			if(input[counter]!=' '){
			c[index] = input[counter];
			//increment index and counter after every iteration.
			index+=1; counter+=1;
			}
			else{counter+=1;}
		}
		//------------------------------
		if(delim==1){
			//printf(1);
			if(input[counter]!= ' '){
			p1[index] = input[counter];	
			
			index+=1; counter+=1;	
			}
			else{counter+=1;}
		}
		//------------------------------
		if(delim==2){
			//printf(2);
			if(input[counter]!= ' '){
			p2[index] = input[counter];
			
			index+=1; counter+=1;
			}
			else{counter+=1;}
		}
		//index+=1; counter+=1;
	}

	//printf("Counter: %d\n", counter);
	//printf("Index: %d\n", index);
	//printf("Delim: %d\n", delim);
	printf("Command: %s\n", c);
	printf("Param1: %s\n", p1);
	printf("Param2: %s\n\n", p2);
}
void breakUp2(char *input, char *c, char *p1, char *p2) {
	int n;
	c[0] = p1[0] = p2[0] = '\0';
	n = sscanf(input, "%s%s%s", c, p1, p2);
}

/*
Interpreter() will take the char arrays initialized in breakUp to interpret.
We use strcmp to decide which of the hard-coded commands will be used to call their respective menu.
We always send the first parameter (second one if requested) to evaluate if user has inputted a param.
If the command did not require a param but one exists (p1[0]!='\0'), then we inform of the error.
This works in the absence of a needed param as well.

*/
int interpreter(char *c, char *p1, char *p2){
	//printf("%s\n", c);
	//printf("%s\n", p1);
	//printf("%s\n", p2);

	//Holds value for the comparison operation.
	int cmp;
	//We will go through the whole list of available commands and evaluate if the c[] array matches. If not, error prompt will display.
	//If matched, we display the appropriate menus and send in the paramaters if any apply.
	cmp = strcmp(c,"load");
	if(cmp == 0){
		if(p1[0]=='\0'){
			if(p2[0]!='\0'){
				printf("load command will only take 1 paramater.\n Use command 'help to list commands available.\n");
			}
			else{
			printf("load command is missing 1/1 parameters.\n Use command 'help' to list commands available.\n");
			}
		}
		else{
			load(p1);
		}
		return 0;
	}
	
	cmp = strcmp(c,"execute");
	if(cmp == 0){
		if(p1[0]=='\0'){
			execute();
		}
		else{
			printf("execute command does not take parameters.\n Use command 'help' to list available commands.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c,"debug");
	if(cmp==0){
		if(p1[0]=='\0'){
			debug();
		}
		else{
			printf("debug command does not take parameters.\n Use command 'help' to list available commands.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c, "dump");
	if(cmp==0){
		//If first parameter is empty
		if(p1[0]=='\0'){
			//If second parameter is empty.
			if(p2[0]=='\0'){
				printf("dump command is missing 2/2 parameters.\n Use command 'help' to list commands available.\n");
			}
			//If second paramter is not empty.
			else if(p2[0] != '\0'){
				printf("dump command is missing the first paramater.\n Use command 'help' to list commands available.\n");
			}
		}
		else if(p2[0]=='\0'){
			//If first parameter is empty.
			if(p1[0]=='\0'){
				printf("dump command is missing 2/2 parameters.\n Use command 'help' to list commands available.\n");
			}
			else if(p1[0] != '\0'){
				printf("dump command is missing the seconds parameter.\n Use command 'help' to list commands available.\n");
			}
		}

		else{
			dump(p1, p2);
		}

		return 0;
	}
	
	cmp = strcmp(c, "help");
	if(cmp==0){
		if(p1[0]=='\0'){
			help();
		}
		else{
			printf("help command does not take parameters.\n use command 'help' to list available commands.\n");
		}
		return 0;
	}
	
	cmp = strcmp(c, "assemble");
	if(cmp == 0){
		if(p1[0]=='\0'){
			if(p2[0]!='\0'){
				printf("assemble command will only take 1 paramater.\n Use command 'help to list commands available.\n");
			}
			else{
			printf("assemble command is missing 1/1 parameters.\n Use command 'help' to list commands available.\n");
			}
		}
		else{
			assemble(p1);
		}
		return 0;
	}
	
	cmp = strcmp(c, "directory");
	if(cmp==0){
		if(p1[0]=='\0'){
			directory();
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
		//Since the exit command exits out of the program, we return 1 to the terminate value.
		return 1;
	}
	
	//Error prompt if all comparison conditions have failed.
	printf("Invalid Input.\n Use command 'help' to list commands available.\n");
	return 0;
}

//-----------------------------------------Main Menu Functions---------------------------------------
void load(char* p1)
{
	printf("Command 'load' is a WIP.\n");
}

void execute()
{
	printf("Command 'execute' is a WIP.\n");
}

void debug()
{
	printf("Command 'debug' is a WIP.\n");
}

void dump(char *p1, char *p2)
{
	printf("Command 'reate_H_Tabledump' is a WIP.\n");
}

void help()
{
	printf("load <filename>\n");
	printf("execute\n");
	printf("debug\n");
	printf("dump <start> <end>\n");
	printf("help\n");
	printf("assemble <filename>\n");
	printf("directory\n");
	printf("exit\n");
}

void assemble(char *filename)
{
	printf("File Chosen: %s\n", filename);
	FILE *pToFile = NULL;
	pToFile = fopen(filename, "r");
	printf("----\n");

	if(pToFile == NULL){
		printf("[ERROR] Cannot Open File. \n");
	}
	else{
		//Create OpCode Table
		OpCodeTable();
		Create_S_Table();

		int line = 0;

		char input[100];
		char p1[10], p2[10], p3[10];

		int locctr;
		int locctr_Flag = 0;
		int s_Address;

		int cmp;

		int _error;
		int loc_Counter;
		int temp;

		int b_Length = 0;
		
		do {
			fgets(input, 100, pToFile);
			line++;

			if (input[0] == '.') {
				continue;
			}
			else {
				newBreakUp(input, p1, p2, p3);
				cmp = strcmp(p2, "START");
				if (cmp == 0) {
					sscanf(p3, "%x", &locctr);
					s_Address = locctr;
					locctr_Flag++;
					_error = 4;
					interpretIntermFile(locctr, Search_H_Table(p2), p3, _error, line);
				}
				else if (locctr_Flag == 0) {
					locctr = 0;
					s_Address = 0;
					locctr_Flag++;
					_error = 4;
					interpretIntermFile(locctr, Search_H_Table(p2), p3, _error, line);
				}
			}
		} while (locctr_Flag == 0);

		while (fgets(input, 100, pToFile)) {
			line++;
			_error = 0;
			b_Length = 0;
			loc_Counter = 0;
			newBreakUp(input, p1, p2, p3);

			cmp = strcmp(p1, "END");
			if (cmp != 0) {
				if (input[0] == '.') {
					continue;
				}
				//Checks if it is a whitespace.
				//Includes /n, /t, etc
				else if (isspace(input[0])) {
					if (Search_H_Table(p1) != 0xFF) {
						//OpCode = WORD
						if (Search_H_Table(p1) == 0xF4) {
							loc_Counter = 3;
							_error = 4;
						}
						//OpCode = RESW
						else if (Search_H_Table(p1) == 0xF6) {
							sscanf(p2, "%d", &temp);
							loc_Counter = temp*3;
							_error = 4;
						}
						//OpCode = RESB
						else if (Search_H_Table(p1) == 0xF5) {
							sscanf(p2, "%d", &temp);
							loc_Counter = temp;
							_error = 4;
						}
						//OpCode = BYTE
						else if (Search_H_Table(p1) == 0xF3) {
							if (p2[0] == 'X') {
								temp = 2;
								while (p2[temp] != 39) {
									temp++;
									b_Length++;
								}

								if (b_Length & 1) {
									loc_Counter = b_Length;
									_error = 2;
								}
								else {
									loc_Counter = b_Length >> 1;
									_error = 4;
								}
							}
							else if (p2[0] == 'C') {
								temp = 2;
								while (p2[temp] != 39) {
									temp++;
									b_Length++;
								}
								loc_Counter = b_Length;
								_error = 4;
							}
						}
						else {
							loc_Counter = 3;
						}
					}
					else {
						_error = 2;
					}
					interpretIntermFile(locctr, Search_H_Table(p1), p2, _error, line);
					locctr += loc_Counter;
					
				}

				else {

					if (Search_S_Table(p1) == 1) {
						_error = 1;
					}
					else {
						Add_S_Item(p1, locctr);
					}

					if (Search_H_Table(p2) != 0xFF) {
						if (Search_H_Table(p2) == 0xF4) {
							loc_Counter = 3;
							_error = 4;
						}
						else if (Search_H_Table(p2) == 0xF6) {
						sscanf(p3, "%d", &temp);
						loc_Counter = temp * 3;
						_error = 4;
					}
						else if (Search_H_Table(p2) == 0xF5) {
							sscanf(p3, "%d", &temp);
							loc_Counter = temp;
							_error = 4;
						}
						else if (Search_H_Table(p2) == 0xF3) {
						if (p3[0] == 'X') {
							temp = 2;
							while (p3[temp] != 39) {
								temp++;
								b_Length++;
							}

							if (b_Length & 1) {
								loc_Counter = b_Length;
								_error = 2;
							}
							else {
								loc_Counter = b_Length >> 1;
								_error = 4;
							}
						}
						else if (p3[0] == 'C') {
							temp = 2;
							while (p3[temp] != 39) {
								temp++;
								b_Length++;
							}
							loc_Counter = b_Length;
							_error = 4;
						}
					}
						else {
							loc_Counter = 3;
						}
					}
					else {
						if (_error == 1) {
							_error = 3;
						}
						else {
							_error = 2;
						}
					}

					interpretIntermFile(locctr, Search_H_Table(p2), p3, _error, line);
					locctr += loc_Counter;
				}
			}
			else {
				_error = 4;
				interpretIntermFile(locctr, Search_H_Table(p1), p2, _error, line);

				goto P1_Exit;
			}
		}
	P1_Exit:
		fclose(pToFile);
		toIntermFile();
		//toSymFile();
		printf("Pass 1 Successful\n");
	}
}

void directory()
{
	printf("Showing Directory\n\n");
	system("ls\n");
	printf("\n");
}
//------------------------------------------------------------------------------------------------------


//-----------------------------------------Phase 2 Append-------------------------------------------------


//As titled, this is the new version of the breakUp function used for the file that will be tested.
//I'll keep the previous version intact to reduce margin of error.
//I don't know why we use the integer "n", but it makes the algorithm work so it stays.
//TODO-> Figure out why it works.
//	-> DRY it up.
void newBreakUp(char *line, char *p1, char *p2, char *p3)
{
	int n;
	p1[0] = p2[0] = p3[0] = '\0';
	n = sscanf(line, "%s%s%s", p1, p2, p3);

}

void toIntermFile()
{
	//Fstream
	FILE *I_File;
	I_File = fopen("intermediate.txt", "w");

	fprintf(I_File, "Location\t |\t OpCode\t |\t Operand\t |\t Error\t |\t Line |\n");

	current = root;

	while (current != NULL) {
		fprintf(I_File, "%x\t\t\t %02x\t\t %s\t\t\t %d\t\t %d \n", current->Location, current->OpCode, current->Operand, current->Error, current->Line);
		current = current->next;
	}
	fclose(I_File);
}

void interpretIntermFile(int Location, int OpCode, char *Operand, int Error, int Line) {
	struct I_Item *new_I_Item = (struct I_Item*) malloc(sizeof(struct I_Item));

	new_I_Item->Location = Location;
	new_I_Item->OpCode = OpCode;
	new_I_Item->Error = Error;
	new_I_Item->Line = Line;
	strncpy(new_I_Item->Operand, Operand, 10);

	new_I_Item->next = NULL;

	if (root == NULL) {
		root = new_I_Item;
	}
	else {
		current = root;
		while (current->next != NULL) {
			current = current->next;
		}

		current->next = new_I_Item;
		current = new_I_Item;
		current->next = NULL;
	}
}


//-----------------------------------------Hash Table Functions---------------------------------------------------
void Create_H_Table()
{
	for (int x = 0; x < 100; x++)
	{
		H_Table[x] = (struct H_Item*) malloc(sizeof(struct H_Item));
		memset(&H_Table[x]->state[0], '\0', sizeof(H_Table[x]->state));
		H_Table[x]->OpCode = 0;
		H_Table[x]->next = NULL;
	}
}

void OpCodeTable()
{
	// initialize the Optable to NULL data
	Create_H_Table();

	// add the items
	Add_H_Item("ADD", 0x18);
	Add_H_Item("AND", 0x58);
	Add_H_Item("COMP", 0x28);
	Add_H_Item("DIV", 0x24);
	Add_H_Item("J", 0x3C);
	Add_H_Item("JEQ", 0x30);
	Add_H_Item("JGT", 0x34);
	Add_H_Item("JLT", 0x38);
	Add_H_Item("JSUB", 0x48);
	Add_H_Item("LDA", 0x00);
	Add_H_Item("LDCH", 0x50);
	Add_H_Item("LDL", 0x08);
	Add_H_Item("LDX", 0x04);
	Add_H_Item("MUL", 0x20);
	Add_H_Item("OR", 0x44);
	Add_H_Item("RD", 0xD8);
	Add_H_Item("RSUB", 0x4C);
	Add_H_Item("STA", 0x0C);
	Add_H_Item("STCH", 0x54);
	Add_H_Item("STL", 0x14);
	Add_H_Item("STX", 0x10);
	Add_H_Item("SUB", 0x1C);
	Add_H_Item("TD", 0xE0);
	Add_H_Item("TIX", 0x2C);
	Add_H_Item("WD", 0xDC);
	// Directives Implement later
	Add_H_Item("START", 0xF1);
	Add_H_Item("END", 0xF2);
	Add_H_Item("BYTE", 0xF3);
	Add_H_Item("WORD", 0xF4);
	Add_H_Item("RESB", 0xF5);
	Add_H_Item("RESW", 0xF6);

}

void Add_H_Item(char *state, int data) {

	int index = HashKey(state);

	if (H_Table[index]->state[0] == '\0') {
		strncpy(H_Table[index]->state, state, 10);
		H_Table[index]->OpCode = data;
		H_Table[index]->next = NULL;
	}
	else {
		struct H_Item* ptr = H_Table[index];
		struct H_Item* new_H_Item = (struct H_Item*) malloc(sizeof(struct H_Item));
		strncpy(new_H_Item->state, state, 10);
		new_H_Item->OpCode = data;
		new_H_Item->next = NULL;

		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = new_H_Item;
	}
}

int Search_H_Table(char *state) {
	int index = HashKey(state);
	int cmp = 0;
	int _OpCode = 0xFF;

	struct H_Item* ptr = H_Table[index];

	while (ptr != NULL) {
		cmp = strcmp(ptr->state, state);

		if (cmp == 0) {
			_OpCode = ptr->OpCode;
			goto jumpOut;
		}
		else {
			ptr = ptr->next;
		}
	}
	jumpOut:
	return _OpCode;
}

int HashKey(char *key) {
	
	int hash = 0;

	for (int x = 0; x < strlen(key); x++) {
		int temp = key[x];
		hash += temp;
	}
	hash %= 100;
	return hash;
}

//---------------------------------------------Symbol Table Functions---------------------------------------------

void Create_S_Table() {
	for (int x = 0; x < 100; x++) {
		S_Table[x] = (struct S_Item*) malloc(sizeof(struct S_Item));
		memset(&S_Table[x]->state[0], '\0', sizeof(S_Table[x]->state));
		S_Table[x]->address = 0;
		S_Table[x]->next = NULL;
	}
}

void Add_S_Item(char *state, int address) {
	int _index = HashKey(state);

	if (S_Table[_index]->state[0] == '\0') {
		strncpy(S_Table[_index]->state, state, 10);
		S_Table[_index]->address = address;
		S_Table[_index]->next = NULL;
	}
	else {
		struct S_Item* ptr = S_Table[_index];
		struct S_Item* new_S_Item = (struct S_Item*) malloc(sizeof(struct S_Item));
		strncpy(new_S_Item->state, state, 10);
		new_S_Item->address = address;
		new_S_Item->next = NULL;
	}
}

int Search_S_Table(char *state) {
	int _index = HashKey(state);
	int _cmp = 0;
	struct S_Item* ptr = S_Table[_index];

	while (ptr != NULL) {
		_cmp = strcmp(ptr->state, state);
		if(_cmp == 0) {
			return 1;
		}
		else {
			ptr = ptr->next;
		}
	}
	return 0;
}





