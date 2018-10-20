//Jafet Reyes
//CSCI 3334

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Libraries Added in Phase 2
#include "ctype.h"
#include "stdbool.h"
//Phase 1 Function Prototypes
//-----------------------------------------------------------------------------------------------
void breakUp(char *, char *, char*, char*);
int interpreter(char *, char *, char *);
//-----------------------------------------------------------------------------------------------

//Phase 2 Function Prototypes
//-Main Menu Functions(load, execute....)
//---------------------------------------
void load(char*);
void execute();
void debug();
void dump(char*, char *);
void help();
void assemble(char*); //Modified for Phase 2.
void directory();
//-------------------------------------------------------------------------------------------
//-Structures
#define T_SIZE 50
#define NULL_OPCODE 0xFF
//--INTERMEDIATE ITEMS
struct I_Item {
	int Location;		//Location Counter
	int OpCode;			//Source Line
	char Operand[10];	//Operand
	int Line;			//Used for formatting
	int Error;			//Error code if any. 
};
struct I_Item *I_Table[T_SIZE];

//--HASH ITEMS
struct H_Item {
	char stm[10];		//Statement
	int OpCode;			//Source line
};
struct H_Item *H_Table[T_SIZE];		//Hash Table. Used to search the OpCode easily.
void OpCodeTable();				//By default, it will add the necessary pre-defined OpCode to the H_Table.
								//-This function will be called by Create_H_Table();

//--SYMBOL ITEMS
struct S_Item {
	char stm[10];		//Statement
	int address;		//Address Counter
};
struct S_Item *S_Table[T_SIZE];		//Symbol Table. Used to hold the actual Symbol Table. Used to output to a file.
//-EOF Structures

//-Intermediate File Functions
//------------------------------------------------------------------------------------------------
void I_BreakUp(char *, char *, char *, char *);					//BreaksUp the original statement and assigns it to variables.
//					(Location|OpCode|Operand|Line|Error)
void Create_I_Table();			//Creates the Intermediate Table and sets most items to Null.
void readIntermData(int, int, char*, int, int);					//Reads in from the source file and sends the broken-up parameters as I_Items
void pToIntermFile();											//Prints out to the "intermediate.txt" file after reading in the new I_Items
void Show_I_Table();
//------------------------------------------------------------------------------------------------

//-Hash_Table Functions
//------------------------------------------------------------------------------------------------
void Show_H_Table();			//TESTING PURPOSES ONLY. DELETE LATER.->Shows the Table Hash Table in it's entirity.
void Create_H_Table();			//Creates the actual Hash Table.
void Add_H_Item();				//Adds an item to the Hash Table.
int Search_H_Table(char *);		//Searches H Table for an item. If found, it shall return a non-0 integer.
int HashKey(char *);			//Returns a key-value used to look for an item in the H-Table, suggested to be easier than binary searches.
//------------------------------------------------------------------------------------------------

//-Symbol Table Functions
//------------------------------------------------------------------------------------------------
void Show_S_Table();			//TESTING PURPOSES ONLY.
void Create_S_Table();			//Creates the actual Symbol Table.
void Add_S_Item();				//Adds an item to the Symbol Table.
int Search_S_Table(char *);		//Searches S_Table for an item. If found, it shall return a non-0 integer.
void pToSymFile();				//Prints out the "symTable.txt" file, as requested.
//------------------------------------------------------------------------------------------------

//PHASE 1-----------------------------------------------------------
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
		breakUp(input, c, p1, p2);
		//After c[], p1[], p2[] hold the command-line parts, we send them over to be
		//...interpreted. The function returns a value, thus we assign it to terminate.
		terminate=interpreter(c, p1, p2);
	}
}
/*
breakUp function will break the command-line (input) into the according variables.
The first char-array will be considered the command, and the subsequent variables will be its params.
We use the same variable names to be consistent and use less brainpower.
//P2 EDIT:
//->Removed the unnecessary algorithm and implemented sscanf
*/
void breakUp(char *input, char *c, char *p1, char *p2){

	//Either of these will reset the arrays to contain nothing, however the for-loop worked before the latter did, so I'm sticking to it.
	for(int x=0; x<=10; x++){c[x]=p1[x]=p2[x] = '\0';}
	//->
	int n = sscanf(input, "%s%s%s", c, p1, p2);
	//Formatting.
	printf("------------------------------------------------\n");
	printf("Command: %s\n", input);

	//printf("Command: %s\n", c);
	//printf("Param1: %s\n", p1);
	//printf("Param2: %s\n\n", p2);
}
/*
Interpreter() will take the char arrays initialized in breakUp to interpret.
We use strcmp to decide which of the hard-coded commands will be used to call their respective menu.
We always send the first parameter (second one if requested) to evaluate if user has inputted a param.
If the command did not require a param but one exists (p1[0]!='\0'), then we inform of the error.
This works in the absence of a needed param as well.

*/
int interpreter(char *c, char *p1, char *p2) {
	//printf("%s\n", c);
	//printf("%s\n", p1);
	//printf("%s\n", p2);

	//Holds value for the comparison operation.
	int cmp;
	//We will go through the whole list of available commands and evaluate if the c[] array matches. If not, error prompt will display.
	//If matched, we display the appropriate menus and send in the paramaters if any apply.
	cmp = strcmp(c, "load");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			if (p2[0] != '\0') {
				printf("load command will only take 1 paramater.\n Use command 'help to list commands available.\n");
			}
			else {
				printf("load command is missing 1/1 parameters.\n Use command 'help' to list commands available.\n");
			}
		}
		else {
			load(p1);
		}
		return 0;
	}

	cmp = strcmp(c, "execute");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			execute();
		}
		else {
			printf("execute command does not take parameters.\n Use command 'help' to list available commands.\n");
		}
		return 0;
	}

	cmp = strcmp(c, "debug");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			debug();
		}
		else {
			printf("debug command does not take parameters.\n Use command 'help' to list available commands.\n");
		}
		return 0;
	}

	cmp = strcmp(c, "dump");
	if (cmp == 0) {
		//If first parameter is empty
		if (p1[0] == '\0') {
			//If second parameter is empty.
			if (p2[0] == '\0') {
				printf("dump command is missing 2/2 parameters.\n Use command 'help' to list commands available.\n");
			}
			//If second paramter is not empty.
			else if (p2[0] != '\0') {
				printf("dump command is missing the first paramater.\n Use command 'help' to list commands available.\n");
			}
		}
		else if (p2[0] == '\0') {
			//If first parameter is empty.
			if (p1[0] == '\0') {
				printf("dump command is missing 2/2 parameters.\n Use command 'help' to list commands available.\n");
			}
			else if (p1[0] != '\0') {
				printf("dump command is missing the seconds parameter.\n Use command 'help' to list commands available.\n");
			}
		}

		else {
			dump(p1, p2);
		}

		return 0;
	}

	cmp = strcmp(c, "help");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			help();
		}
		else {
			printf("help command does not take parameters.\n use command 'help' to list available commands.\n");
		}
		return 0;
	}

	cmp = strcmp(c, "assemble");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			if (p2[0] != '\0') {
				printf("assemble command will only take 1 paramater.\n Use command 'help to list commands available.\n");
			}
			else {
				printf("assemble command is missing 1/1 parameters.\n Use command 'help' to list commands available.\n");
			}
		}
		else {
			assemble(p1);
		}
		return 0;
	}

	cmp = strcmp(c, "directory");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			directory();
		}
		else {
			printf("directory command does not take paramaters.\n Use command 'help' to list commands available.\n");
		}
		return 0;
	}

	cmp = strcmp(c, "exit");
	if (cmp == 0) {
		if (p1[0] == '\0') {
			printf("Exiting Program...\n");
		}
		else {
			printf("exit command does not take paramaters.\n Use command 'help' to list commands available.\n");
		}
		//Since the exit command exits out of the program, we return 1 to the terminate value.
		return 1;
	}

	//Error prompt if all comparison conditions have failed.
	printf("Invalid Input.\n Use command 'help' to list commands available.\n");
	return 0;
}

//Interpreter Functions--------------------
void load(char* filename)
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

//MODIFIED FOR PHASE 2.
void assemble(char *filename) {

	printf("File Chosen: %s\n", filename);
	FILE *file = fopen(filename, "r");
	printf("-----------------\n");
	if (file == NULL) {
		printf("[ERROR] Cannot Open File.\n");
	}
	else {
		//Set Up our tables to write in.
		OpCodeTable();		//OpCode is predetermined. We're reading more from it than adding to it.
		Create_S_Table();	//Symbol Table will be written to through most of this program.
		Create_I_Table();

		//------------------------------------------------------------------------------------------
		char _stm[100];
		char _p1[10], _p2[10], _p3[10];
		int _line = 0;		//Used to visualize the line of the source file we are in.

		int cmp;

		int locCtr;
		int d_locCtr = 0;
		int startAddr;
		int _code;			//Used for error codes.

		int temp;
		int _location;
		int _byte;
		//------------------------------------------------------------------------------------------
		//STEPS:
			//[1] Initial Setup. Check if the line read is a comment.
			//[2] Check if the statement field is empty.
			//[3] If neither of these, use all the parameters.

		//[1]
		do {
			fgets(_stm, 100, file);
			_line++;
			//Immediately check if the line is a comment. If so, skip it.
			if (_stm[0] == '.') {
				continue;
			}
			else {
				//Seperate the input into the seperate variables.
				I_BreakUp(_stm, _p1, _p2, _p3);
				//Once the line has broken up, check if the second variable holds the START directive.
				cmp = strcmp(_p2, "START");
				if (cmp == 0) {
					sscanf(_p3, "%x", &locCtr);
					startAddr = locCtr;
					d_locCtr++;
					_code = 4;
					readIntermData(locCtr, Search_H_Table(_p2), _p3, _line, _code);
				}
				else if (d_locCtr == 0) {
					locCtr = startAddr = 0;
					d_locCtr++;
					_code = 4;
					//Send data to be added to the I_Table array.
					readIntermData(locCtr, Search_H_Table(_p2), _p3, _line, _code);
				}
			}
		} while (d_locCtr == 0);

		//Initial conditions have been checked for at this point.


		while (fgets(_stm, 100, file)) {
			_code = 0;
			_byte = 0;
			_location = 0;

			I_BreakUp(_stm, _p1, _p2, _p3);

			cmp = strcmp(_p1, "END");
			if (cmp != 0) {

				if (_stm[0] == '.') {
					//continue
				}
				//[2]
				//Statement field is empty, therefore p1 and p2 are evaluated.
				else if (isspace(_stm[0])) {

					//If the search returns a value that is not 0xFF, then proceed.
					//This essentially means it found an item.
					if (Search_H_Table(_p1) != NULL_OPCODE) {
						int _OpCode = Search_H_Table(_p1);

						//WORD
						if (_OpCode == 0xF4) {
							_location = 3;
							_code = 4;
						}
						//RESW
						else if (_OpCode == 0xF6) {
							sscanf(_p2, "%d", &temp);
							_location = 3 * temp;
							_code = 4;
						}
						//RESB
						else if (_OpCode == 0xF5) {
							sscanf(_p2, "%d", &temp);
							_location = 3 * temp;
							_code = 4;
						}
						//BYTE
						else if (_OpCode == 0xF3) {
							//Find length of the bytes.
							//Add to locCtr
							if (_p2[0] == 'C') {
								temp = 2;
								while (_p2[temp] != 39) {
									_byte++;
									temp++;
								}
								_location = _byte;
								_code = 4;
							}
							else if (_p2[0] == 'X') {
								temp = 2;
								while (_p2[temp] != 39) {
									_byte++;
									temp++;
								}

								if (_byte & 1) {
									_location = _byte;
									_code = 2;
								}
								else {
									_location = _byte >> 1;
									_code = 4;
								}
							}
						}
						//Normal Mnemonic.
						else {
							_location = 3;
						}
					}

					//If p1 is not found within the H_Table, set the code to 2 and continue with reading the data.
					else {
						_code = 2;
					}
					readIntermData(locCtr, Search_H_Table(_p1), _p2, _line, _code);
					locCtr += _location;
				}
				//[3]
				//If the first statement is not empty, then evalue p1 p2 and p3
				else {
					//If p1 is found within the Symbol Table, then it is a duplicate. Flag as an error.
					if (Search_S_Table(_p1) == 1) {
						_code = 1;
					}
					//Otherwise, add it to the S_Table.
					else {
						Add_S_Item(_p1, locCtr);
					}

					//OPCODES

					if (Search_H_Table(_p2) != 0xFF) {
						int _OpCode = Search_H_Table(_p2);

						//WORD
						if (_OpCode == 0xF4) {
							_location = 3;
							_code = 4;
						}
						//RESW
						else if (_OpCode == 0xF6) {
							sscanf(_p3, "%d", &temp);
							_location = 3 * temp;
							_code = 4;
						}
						//RESB
						else if (_OpCode == 0xF5) {
							sscanf(_p3, "%d", &temp);
							_location = temp;
							_code = 4;
						}
						//BYTE
						else if (_OpCode == 0xF3) {
							//Same as before.
							if (_p3[0] == 'C') {
								temp = 2;
								while (_p3[temp] != 39) {
									_byte++;
									temp++;
								}
								_location = _byte;
								_code = 4;
							}
							else if (_p3[0] == 'X') {
								temp = 2;
								while (_p3[temp] != 39) {
									_byte++;
									temp++;
								}
								if (_byte & 1) {
									_location = _byte;
									_code = 2;
								}
								else {
									_location = _byte >> 1;
									_code = 4;
								}
							}
						}

						//Normal mnemonic.
						else {
							_location = 3;
						}
					}
					else {
						if (_code == 1) {
							_code = 3;
						}
						else {
							_code = 2;
						}
					}
					readIntermData(locCtr, Search_H_Table(_p2), _p3, _line, _code);
					locCtr += _location;
				}

				}
			else {
				_code = 4;
				int _OpCode = Search_H_Table(_p1);
				readIntermData(locCtr, _OpCode, _p2, _line, _code);
				goto P1_Exit;
			}
		}

	P1_Exit:
		fclose(file);
		/*
		pToIntermFile();
		Show_H_Table();
		printf("==================================================================\n");
		Show_S_Table();
		printf("==================================================================\n");
		Show_I_Table();
		printf("Intermediate.txt && SymFile.txt Created\n");
		*/
		pToIntermFile();
		pToSymFile();
	}
}

void directory()
{
	printf("Showing Directory\n\n");
	system("ls\n");
	printf("\n");
}
//-----------------------EOF Interpreter Functions

//-----------------------------------------------------------------END PHASE 1


//PHASE2------------------------------------------------------------

//-------------------Intermediate File Functions------------------------------
void I_BreakUp(char *stm, char *p1, char *p2, char *p3) {
	int n;
	p1[0] = p2[0] = p3[0] = '\0';
	n = sscanf(stm, "%s%s%s", p1, p2, p3);
}

void Create_I_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		I_Table[x] = (struct I_Item*) malloc(sizeof(struct I_Item));
		I_Table[x]->Location = 0;
		I_Table[x]->OpCode = 0;
		memset(&I_Table[x]->Operand[0], '\0', sizeof(I_Table[x]->Operand));
		I_Table[x]->Error = 0;
		I_Table[x]->Line = 0;
	}
}

void Show_I_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		printf("-------------------------\n");
		printf("Location: %d\t Opcode: %d\t Operand: %s\t Line: %d\t Error: %d\n)", I_Table[x]->Location, I_Table[x]->OpCode, I_Table[x]->Operand, I_Table[x]->Error, I_Table[x]->Line);
	}
}

void readIntermData(int Location, int OpCode, char *Operand, int Line, int Error) {
	int index = 0;
	while (I_Table[index]->Operand[0] != '\0') {
		index++;
	}
	I_Table[index]->Location = Location;
	I_Table[index]->OpCode = OpCode;
	strncpy(I_Table[index]->Operand, Operand, 10);
	I_Table[index]->Line = Line;
	I_Table[index]->Error = Error;
}

void pToIntermFile() {
	FILE *I_File = fopen("intermediate.txt", "w");
	fprintf(I_File, "Location\t |\t OpCode\t |\t Operand\t |\t\t Line\t |\t Error\t |\n");
	for (int x = 0; x < T_SIZE; x++) {
		//int _Location = I_Table[x]->Location;
		//int _OpCode = I_Table[x]->OpCode;
		//char _Operand = I_Table[x]->Operand;
		//int _Line = I_Table[x]->Line;
		//int _Error = I_Table[x]->Error;

		//fprintf(I_File, "%x \t\t %02x \t\t %s \t\t %d \t\t %d \n", _Location, _OpCode, _Operand, _Line, _Error);
		fprintf(I_File, "%x\t\t\t %#x\t\t %-8s\t\t\t %d\t\t %d\n", I_Table[x]->Location, I_Table[x]->OpCode, I_Table[x]->Operand, I_Table[x]->Line, I_Table[x]->Error);
	}
	fclose(I_File);
}

//-------------------Hash Table Functions--------------------------------------
void Create_H_Table() {			//Initiliaze the H_Table[] Array to = '0'. Will be used as a condition to where the "top" of the array is
	for (int x = 0; x < T_SIZE; x++) {
		H_Table[x] = (struct H_Item*) malloc(sizeof(struct H_Item));
		memset(&H_Table[x]->stm[0], '\0', sizeof(H_Table[x]->stm));
		H_Table[x]->OpCode = 0;
	}

	//DO NOT DELETE. ADD ITEMS LIKE THIS.
	//strncpy(H_Table[10]->stm, "NEW", 10);
	//H_Table[10]->OpCode = 7;
}

void Add_H_Item(char *stm, int OpCode) {
	int index = 0;
	while (H_Table[index]->stm[0] != '\0') {
		index++;
	}
	strncpy(H_Table[index]->stm, stm, 10);
	H_Table[index]->OpCode = OpCode;
	
}

void OpCodeTable() {

	Create_H_Table();		//Initializes the table and sets all the values to a default. We add the predetermined code.

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

void Show_H_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		printf("------------------------\n");
		printf("\tINDEX: %d\n", x);
		printf("Stm: %-10s\t OpCode: %#x\n",H_Table[x]->stm,H_Table[x]->OpCode);
	}
}

int Search_H_Table(char* stm) {
	int cmp = 0;
	int index = 0;
	int _OpCode = NULL_OPCODE;		//No other item in the list contains this OpCode. Therefore, if an item is not found, it shall return this instead.
	while (H_Table[index]->stm != '\0') {
		cmp = strcmp(H_Table[index]->stm, stm);
		if (cmp == 0) {
			_OpCode = H_Table[index]->OpCode;		//If it IS found, we redefine it and jump out of the loop to return the assigned value.
			goto Found;
		}
		else {
			index++;
		}
	}
	Found:
	return _OpCode;
}

//------------------Symbol Table Functions--------------------------------------
void Create_S_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		S_Table[x] = (struct S_Item*) malloc(sizeof(struct S_Item));
		memset(&S_Table[x]->stm[0], '\0', sizeof(S_Table[x]->stm));
		S_Table[x]->address = 0;
	}
}

void Add_S_Item(char *stm, int address) {
	int index = 0;
	while (S_Table[index]->stm[0] != '\0') {
		index++;
	}
	strncpy(S_Table[index]->stm, stm, 10);
	S_Table[index]->address = address;
}

void Show_S_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		printf("------------------------\n");
		printf("\tINDEX: %d\n", x);
		printf("Stm: %-10s\t Address: %d\n", S_Table[x]->stm, S_Table[x]->address);
	}
}

int Search_S_Table(char *stm) {
	int cmp = 0;
	int index = 0;
	int _found = 0;
	while (S_Table[index]->stm != '\0') {
		cmp = strcmp(S_Table[index]->stm, stm);
		if (cmp == 0) {
			_found = 1;
			goto S_Found;
		}
		else {
			index++;
		}
	}
	S_Found:
	return _found;
}

void pToSymFile() {
	FILE *S_File = fopen("symTable.txt", "w");
	for (int x = 0; x < T_SIZE-1; x++) {
		fprintf(S_File, "%s\t\t %d\n", S_Table[x]->stm, S_Table[x]->address);
	}
	fclose(S_File);
}
//-----------------------------------------------------------------END PHASE 2

