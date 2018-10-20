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
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
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
#define T_SIZE 55
#define NULL_OPCODE 0xFF
//--INTERMEDIATE ITEMS
struct I_Item {
	char Source[100];	//Added v2
	int Location;		//Location Counter
	int OpCode;			//Source Line
	char Operand[10];	//Operand
	int Line;			//Used for formatting
	int Error;			//Error code if any.
	//PHASE 3 MODIFICATION
	//Added a property to store Comments for the intermediate File.
	char Comment[30];
};
struct I_Item *I_Table[T_SIZE];

//--OPTABLE ITEMS
struct O_Item {
	char stm[10];		//Statement
	int OpCode;			//Source line
};
struct O_Item *O_Table[T_SIZE];		//OpTable. Used to search the OpCode easily.
void OpCodeTable();				//By default, it will add the necessary pre-defined OpCode to the O_Table.
								//-This function will be called by Create_O_Table();

//--SYMBOL ITEMS
struct S_Item {
	char stm[10];		//Statement
	int address;		//Address Counter
};
struct S_Item *S_Table[T_SIZE];		//Symbol Table. Used to hold the actual Symbol Table. Used to output to a file.

//--OBJECT ITEM
struct Obj_Item{
	int OpCode;
	char Address[5];
	char Object[7];
	int Used;
};
struct Obj_Item *Obj_Table[T_SIZE];

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//--------------[Intermediate File Functions]--------------------
//------------------------------------------------------------------------------------------------
void I_BreakUp(char *, char *, char *, char *, char *);					//BreaksUp the original statement and assigns it to variables.
void Create_I_Table();			//Creates the Intermediate Table and sets most items to Null.
//					(Location|OpCode|Operand|Line|Error)
void readIntermData(char *, int, int, char*, int, int, char *);					//Reads in from the source file and sends the broken-up parameters as I_Items
void pToIntermFile();											//Prints out to the "intermediate.txt" file after reading in the new I_Items
void Show_I_Table();
//------------------------------------------------------------------------------------------------
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//-OpCode Table Functions
//------------------------------------------------------------------------------------------------
void Create_O_Table();			//Creates the actual Hash Table.
void Add_O_Item();				//Adds an item to the Hash Table.
int Search_O_Table(char *);		//Searches H Table for an item. If found, it shall return a non-0 integer.
//------------------------------------------------------------------------------------------------
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//-Symbol Table Functions
//------------------------------------------------------------------------------------------------
void Create_S_Table();			//Creates the actual Symbol Table.
void Add_S_Item();				//Adds an item to the Symbol Table.
int Search_S_Table(char *);		//Searches S_Table for an item. If found, it shall return a non-0 integer.
void pToSymFile();				//Prints out the "symTable.txt" file, as requested.
//------------------------------------------------------------------------------------------------
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//-Phase 3 Function Prototypes
//-------------------------------------------------------------------------------------------------
void PassTwo();					//Main function where Phase 3 will occur. Used to seperate from the already messy Phase 2.

void Create_Obj_Table();
void CreateObjectCode();
void Add_Obj_Item(char *);
int getSymbolAddress(int, char*);
//--------------------------------------------------------------------------------EOF PHASE 3 PROTOTYPES
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
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
	printf("Command 'dump' is a WIP.\n");
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

void directory()
{
	printf("Showing Directory\n\n");
	system("ls\n");
	printf("\n");
}

//MODIFIED FOR PHASE 2.
//REWORKED FOR PHASE 3.
int prog_length;		//Will store the program length after pass one. (LOCCTR-StartAddress)
void assemble(char *filename) {
	//-----------------------------PASS ONE------------------------------------------------------------------
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
		char _source[100];		//Holds the original source line to be broken up to the next variables.
		char _label[10], _mnemonic[10], _operand[10], _comment[30];
		int _line = 0;		//Used to visualize the line of the source file we are in.

		int cmp;

		int LOCCTR;
		int locctr_found = 0;		//Used for Step[1]. Determines if 'START' directive has been found.
		int startAddr;
		int _code;			//Used for error codes.

		int temp;
		int LOC_INCR;			//Used to add upon the locCtr
		int _byte;
		//------------------------------------------------------------------------------------------
		//STEPS
		//[1] Find Starting Address
			//We do not proceed with LOCCTR increments until we find the 'START' directive.
		//[2] Proceed with assigning Addresses and creating SymTable

		//[1]
		do {
			fgets(_source, 100, file);
			_line++;
			I_BreakUp(_source, _label, _mnemonic, _operand, _comment);
			if (_source[0] == '.') {
				readIntermData(_source, LOCCTR, Search_O_Table(_mnemonic), _operand, _line, _code, _comment);
			}
			else{
				cmp = strcmp(_mnemonic, "START");
				if (cmp == 0) {
					sscanf(_operand, "%x", &LOCCTR);
					startAddr = LOCCTR;
				}
				else if (locctr_found == 0) {
					LOCCTR = startAddr = 0;
				}
				locctr_found++;
				_code = 4;

				/*if (_label[0] != '\0')
				{
					if (Search_S_Table(_label) == 1) {
						_code = 1;
					}
					else {
						Add_S_Item(_label, LOCCTR);
					}
				}*/
				readIntermData(_source, LOCCTR, Search_O_Table(_mnemonic), _operand, _line, _code, _comment);
			}

		} while (locctr_found == 0);

		//[2]
		while (fgets(_source, 100, file)) {
			_line++;
			_code = _byte = LOC_INCR = 0;
			I_BreakUp(_source, _label, _mnemonic, _operand, _comment);

			cmp = strcmp(_mnemonic, "END");
			if (cmp != 0) {
				//Immediately check that, if in the presence of a label, it does not already exist within the symbol table.
				//If Label exists and it appears within the SymbolTable, flag as Code 1 (Duplicate).
				if (_label[0] != '\0')
				{
					if (Search_S_Table(_label) == 1) {
						_code = 1;
					}
					//Otherwise, add it immediately before proceeding to increment the LOCCTR for the NEXT address.
					else {
						Add_S_Item(_label, LOCCTR);
					}
				}
				//If the whole line is a comment
				if (_source[0] == '.') {
					readIntermData(_source, LOCCTR, Search_O_Table(_mnemonic), _operand, _line, _code, _comment);
				}
				else {
					//Search the OpCode Table for the equivalent OpCode of the Mnemonic.
					int _OpCode = Search_O_Table(_mnemonic);
					//If it is not found, it will return the NULL_OPCODE definition.
					//Otherwise, mnemonic exists and we can proceed to evaluations.
					if (_OpCode != NULL_OPCODE) {
						//SIC DIRECTIVES
						//WORD -> +3 to LOCCTR
						if (_OpCode == 0xF4) {
							LOC_INCR = 3;
							_code = 4;
						}
						//RESB -> +Operand to LOCCTR
						else if (_OpCode == 0xF5) {
							sscanf(_operand, "%d", &temp);
							LOC_INCR = temp;
							_code = 4;
						}
						//RESW -> +(Operand*3) to LOCCTR
						else if (_OpCode == 0xF6) {
							sscanf(_operand, "%d", &temp);
							LOC_INCR = temp * 3;
							_code = 4;
						}
						//BYTE
						else if (_OpCode == 0xF3) {
							//Find length of the bytes.
							//Add to locCtr
							if (_operand[0] == 'C') {
								temp = 2;
								while (_operand[temp] != 39) {
									_byte++;
									temp++;
								}
								LOC_INCR = _byte;
								_code = 4;
							}
							else if (_operand[0] == 'X') {
								temp = 2;
								//ASCII 39 == ' \' '
								//We could do the aforementioned, but a number hurts less to use.
								while (_operand[temp] != 39) {
									_byte++;
									temp++;
								}
								//Bitwise AND(&&) operation.
								if (_byte & 1) {
									LOC_INCR = _byte;
									_code = 2;
								}
								//Shift the location counter.
								else {
									LOC_INCR = _byte >> 1;
									_code = 4;
								}
							}
						}
						//Normal Mnemonic
						else {
							LOC_INCR = 3;
						}
						
					}

					else {
						_code = 2;
					}
					readIntermData(_source, LOCCTR, _OpCode, _operand, _line, _code, _comment);
					LOCCTR += LOC_INCR;
				}
			}

			//If "END" directive is found
			else {
				_code = 4;
				int _OpCode = Search_O_Table(_mnemonic);
				readIntermData(_source, LOCCTR, _OpCode, _operand, _line, _code, _comment);
				goto P1_Exit;
			}
		}
	P1_Exit:
		fclose(file);
		
		printf("Intermediate.txt & SymFile.txt Created\n");
		printf("Pass 1 Complete\n");
		
		//Show_I_Table();


		prog_length = LOCCTR - startAddr;
		unsigned int retadr = I_Table[16]->Location - startAddr;
		char r_len[5];
		int red;
		sprintf(r_len, "%x", retadr);
		sscanf(r_len, "%d", &red);
		//printf("%X\n", red);
		pToIntermFile();
		pToSymFile();
	}
	//----------------------------END OF PASS ONE------------------------------------------------------------


	//-----------------------------PASS TWO------------------------------------------------------------------
	//PassTwo();
	CreateObjectCode();
}

//-----------------------EOF Interpreter Functions

//-----------------------------------------------------------------END PHASE 1
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//PHASE2------------------------------------------------------------

//-------------------Intermediate File Functions------------------------------
//REWORKED for Phase 3.
//Will now also store the comment field found from the Source file.
//Will consider the use of RSUB
//Will consider if a whole line is a comment (previously done in assemble()
void I_BreakUp(char *Source, char *Label, char *Mnemonic, char *Operand, char *Comment) {
	int n;
	Label[0] = Mnemonic[0] = Operand[0] = Comment[0] = '\0';
	//If Label is Empty
	if (isspace(Source[0])) {
		n = sscanf(Source, "%s%s\t\t%[^\n]s", Mnemonic, Operand, Comment);
		//If RSUB is present
		if (Search_O_Table(Mnemonic) == 0x4c) {
			Operand[0] = '\0';
			n = sscanf(Source, "\t%s\t\t%[^\n]s", Mnemonic, Comment);
		}
	}
	else if (Source[0] == '.') {
		strncpy(Comment, Source, 30);
	}
	//else everything works.
	else {
		n = sscanf(Source, "%s%s%s\t\t%[^\n]s", Label, Mnemonic, Operand, Comment);
	}
}

void Create_I_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		I_Table[x] = (struct I_Item*) malloc(sizeof(struct I_Item));
		I_Table[x]->Location = 0;
		I_Table[x]->OpCode = NULL_OPCODE;
		memset(&I_Table[x]->Operand[0], '\0', sizeof(I_Table[x]->Operand));
		I_Table[x]->Error = 0;
		I_Table[x]->Line = 0;
		memset(&I_Table[x]->Comment[0], '\0', sizeof(I_Table[x]->Comment));
	}
}
void Show_I_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		printf("-------------------------\n");
		printf("Location: %d\t Opcode: %#x\t Operand: %s\t Line: %d\t Error: %d\t Comment: %s\n)", I_Table[x]->Location, I_Table[x]->OpCode, I_Table[x]->Operand, I_Table[x]->Error, I_Table[x]->Line, I_Table[x]->Comment);
	}
}
void readIntermData(char *Source, int Location, int OpCode, char *Operand, int Line, int Error, char *Comment) {
	int index = 0;
	if (Line < T_SIZE) {
		index = Line - 1;
		strncpy(I_Table[index]->Source, Source, 100);
		I_Table[index]->Location = Location;
		I_Table[index]->OpCode = OpCode;
		strncpy(I_Table[index]->Operand, Operand, 10);
		I_Table[index]->Line = Line;
		I_Table[index]->Error = Error;
		strncpy(I_Table[index]->Comment, Comment, 30);
	}
}

void pToIntermFile() {
	FILE *I_File = fopen("intermediate.txt", "w");
	fprintf(I_File, "----------------------------------------------------------------------------------------------------------\n");
	fprintf(I_File, "Line\t |\t OpCode\t |\t Operand\t |\t\t Location\t |\t Error\t |\t\t\t Comment\t\t \n");
	for (int x = 0; x < T_SIZE; x++) {
		fprintf(I_File, "%d\t\t %#04X\t\t %-8s\t\t\t %X\t\t\t %d\t\t %s\t\t \n", I_Table[x]->Line, I_Table[x]->OpCode, I_Table[x]->Operand, I_Table[x]->Location, I_Table[x]->Error, I_Table[x]->Comment);
	}
	fprintf(I_File, "\n Codes:\n");
	fprintf(I_File, "----------------------------\n");
	fprintf(I_File, "[0]: No Error\n");
	fprintf(I_File, "[1]: Symbol Table Contains Duplicate Label.\n");
	fprintf(I_File, "[2]: Error in OpCode (incorrect spelling, byte length error)\n");
	fprintf(I_File, "[3]: Critical Error - Both [1] and [2] Occurred.\n");
	fprintf(I_File, "[4]: Directive Found.\n");

	fclose(I_File);
}

//-------------------Hash Table Functions--------------------------------------
void Create_O_Table() {			//Initiliaze the O_Table[] Array to = '0'. Will be used as a condition to where the "top" of the array is
	for (int x = 0; x < T_SIZE; x++) {
		O_Table[x] = (struct O_Item*) malloc(sizeof(struct O_Item));
		memset(&O_Table[x]->stm[0], '\0', sizeof(O_Table[x]->stm));
		O_Table[x]->OpCode = 0;
	}

	//DO NOT DELETE. ADD ITEMS LIKE THIS.
	//strncpy(O_Table[10]->stm, "NEW", 10);
	//O_Table[10]->OpCode = 7;
}

void Add_O_Item(char *stm, int OpCode) {
	int index = 0;
	while (O_Table[index]->stm[0] != '\0') {
		index++;
	}
	strncpy(O_Table[index]->stm, stm, 10);
	O_Table[index]->OpCode = OpCode;
	
}

void OpCodeTable() {

	Create_O_Table();		//Initializes the table and sets all the values to a default. We add the predetermined code.

	Add_O_Item("ADD", 0x18);
	Add_O_Item("AND", 0x58);
	Add_O_Item("COMP", 0x28);
	Add_O_Item("DIV", 0x24);
	Add_O_Item("J", 0x3C);
	Add_O_Item("JEQ", 0x30);
	Add_O_Item("JGT", 0x34);
	Add_O_Item("JLT", 0x38);
	Add_O_Item("JSUB", 0x48);
	Add_O_Item("LDA", 0x00);
	Add_O_Item("LDCH", 0x50);
	Add_O_Item("LDL", 0x08);
	Add_O_Item("LDX", 0x04);
	Add_O_Item("MUL", 0x20);
	Add_O_Item("OR", 0x44);
	Add_O_Item("RD", 0xD8);
	Add_O_Item("RSUB", 0x4C);
	Add_O_Item("STA", 0x0C);
	Add_O_Item("STCH", 0x54);
	Add_O_Item("STL", 0x14);
	Add_O_Item("STX", 0x10);
	Add_O_Item("SUB", 0x1C);
	Add_O_Item("TD", 0xE0);
	Add_O_Item("TIX", 0x2C);
	Add_O_Item("WD", 0xDC);
	// Directives
	Add_O_Item("START", 0xF1);
	Add_O_Item("END", 0xF2);
	Add_O_Item("BYTE", 0xF3);
	Add_O_Item("WORD", 0xF4);
	Add_O_Item("RESB", 0xF5);
	Add_O_Item("RESW", 0xF6);
}


int Search_O_Table(char* stm) {
	int cmp = 0;
	int index = 0;
	int _OpCode = NULL_OPCODE;		//No other item in the list contains this OpCode. Therefore, if an item is not found, it shall return this instead.
	while (O_Table[index]->stm != '\0') {
		cmp = strcmp(O_Table[index]->stm, stm);
		if (cmp == 0) {
			_OpCode = O_Table[index]->OpCode;		//If it IS found, we redefine it and jump out of the loop to return the assigned value.
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
		fprintf(S_File, "%s\t\t %x\n", S_Table[x]->stm, S_Table[x]->address);
	}
	fclose(S_File);
}
//-----------------------------------------------------------------END PHASE 2
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//PHASE 3--------------------------------------------------------------------------------------------------------
void Create_Obj_Table() {
	for (int x = 0; x < T_SIZE; x++) {
		Obj_Table[x] = (struct Obj_Item*) malloc(sizeof(struct Obj_Item));
		//memset(&I_Table[x]->Operand[0], '\0', sizeof(I_Table[x]->Operand));
		for (int y = 0; y < 7; y++) {
			/*if (y != 6) {
				memset(&Obj_Table[x]->Object[y], '0', sizeof(Obj_Table[x]->Object));
				memset(&Obj_Table[x]->Address[y], '0', sizeof(Obj_Table[x]->Address));
				Obj_Table[x]->OpCode = 0;
				Obj_Table[x]->Used = 0;
			}
			else {
				memset(&Obj_Table[x]->Object[y], '\0', sizeof(Obj_Table[x]->Object));
				memset(&Obj_Table[x]->Address[y], '\0', sizeof(Obj_Table[x]->Address));
				Obj_Table[x]->OpCode = 0;
				Obj_Table[x]->Used = 0;
			}*/
			memset(&Obj_Table[x]->Object[y], '\0', sizeof(Obj_Table[x]->Object));
			memset(&Obj_Table[x]->Address[y], '\0', sizeof(Obj_Table[x]->Address));
			Obj_Table[x]->OpCode = 0;
			Obj_Table[x]->Used = 0;
		}
	}
}

void CreateObjectCode() {
	Create_Obj_Table();

	FILE *OBJECT_FILE = fopen("object.txt", "w");
	for (int x = 0; x < T_SIZE; x++) {
		int _opcode = I_Table[x]->OpCode;
		char _operand[10];
		char _object[10];
		_operand[0] = _object[0] = '\0';
		strcpy(_operand, I_Table[x]->Operand);
		char _address[5];
		if (_opcode == 0xF5 || _opcode == 0xF6) {
			//sprintf(_object, "%02X%s", _opcode, _address);
		}
		else if (I_Table[x]->Comment[0] == '.') {

		}
		//If BYTE
		else if (_opcode == 0xF3) {
			int temp = 2;
			//If C -> Get Constant, Convert to ASCII, set as ObjectCode
			if (_operand[0] == 'C') {
				//printf("BYTE-C\n");
				while (_operand[temp] != 39) {	
					char temp_string[3];
					strncpy(temp_string, _operand, sizeof(_operand));
					sprintf(temp_string, "%X", _operand[temp]);
					strcat(_object, temp_string);
					temp++;
				}
				//printf("%s\n", _object);
			}
			//IF X -> Get Constant and set as ObjectCode
			if (_operand[0] == 'X') {
				//printf("BYTE-X\n");
				while (_operand[temp] != 39) {
					char temp_string[3];
					strncpy(temp_string, _operand, sizeof(_operand));
					sprintf(temp_string, "%c", _operand[temp]);
					strcat(_object, temp_string);
					temp++;
				}
				//printf("%s\n", _object);
			}
		}
		//If WORD
		else if (_opcode == 0xF4) {
			//Send Operand as Hex as ObjectCode
			//printf("WORD FOUND\n");
			int n;
			sscanf(_operand, "%d", &n);
			sprintf(_object, "%06x", n);

			//printf("%s\n", _object);
		}
		else {
			sprintf(_address, "%04X", getSymbolAddress(_opcode, _operand));
			sprintf(_object, "%02X%s", _opcode, _address);
		}

		Add_Obj_Item(_object);
		fprintf(OBJECT_FILE,"%s\n",Obj_Table[x]->Object);

	}
	fclose(OBJECT_FILE);
}

void Add_Obj_Item(char *object) {
	int index = 0;
	while (Obj_Table[index]->Used != 0) {
		index++;
	}
	strncpy(Obj_Table[index]->Object, object, 10);
	Obj_Table[index]->Used = 1;
}

int getSymbolAddress(int OpCode, char *Operand) {
	int index = 0;
	int cmp;
	int address_increment=0;
	cmp = strcmp(Operand, "BUFFER,X");
	if (cmp == 0) {
		strcpy(Operand, "BUFFER");
		//We're adding 8000 to 1xxx due to the indexing.
		//8000(d) = 32768(h)
		address_increment = 32768;
	}

	while (S_Table[index]->stm != '\0' && index<50) {
		cmp = strcmp(S_Table[index]->stm, Operand);
		if (cmp == 0) {
			return S_Table[index]->address + address_increment;
		}
		index++;
	}
	return 0;
}
//----------------------------------------------------------------------------EOF PHASE 3

