#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#define TYPE_SIZE 1
#define TYPE_ONE 0
#define TYPE_TWO 1
#define TYPE_ONE_AMOUNT_SIZE 1
#define TYPE_TWO_AMOUNT_SIZE 3
#define MAX_DIGITS 5

uint16_t readTypeOneNum(FILE* );

unsigned char readTypeOneAmount(FILE*);

uint32_t readTypeTwoAmount(FILE* );

unsigned char readType(FILE*);

bool notEndOfFile(FILE*);

int readTypeTwoInt(FILE*);

void writeFileAndConsole(uint32_t);

bool isEndOfNumber(unsigned char);


int main(int argc, char **argv) {
	/*if (argc != 3) {
		printf("Need two args. %d provided", argc - 1);
		exit(1);
	}*/

	FILE* inFile = NULL;
	//inFile = fopen(argv[1], "rb");
	fopen_s(&inFile, "file1.bin", "rb");

	FILE* outFile = NULL;
	//outFile = fopen(argv[2], "w");
	fopen_s(&outFile, "lol.txt", "w");

	if (!inFile && !outFile) {
		printf("Cannot open 1 or more files");
		exit(1);
	}

	while (notEndOfFile(file)) {
		unsigned char type = readType(file);
		if (type == TYPE_ONE) {
			unsigned char amount = readTypeOneAmount(file);
			writeFileAndConsole(amount);
			for (int i = 0; i < amount - 1; i++) {
				uint16_t number = readTypeOneNum(inFile);
				writeFileAndConsole(number);
			}
			uint16_t number = readTypeOneNum(inFile);
			writeFileAndConsole(number);
		}
		else if (type == TYPE_TWO){
			uint32_t amount = readTypeTwoAmount(inFile);
			writeFileAndConsole(amount);
			for (int i = 0; i < amount - 1; i++) {
				int number = readTypeTwoInt(inFile);
				writeFileAndConsole(number);
			}
			int number = readTypeTwoInt(inFile);
			writeFileAndConsole(number);
		}
	}
	fclose(inFile);
	fclose(outFile);
}

/*
  Read two bytes from file and
  returns a 16 bit integer.
  Increments the file pointer in the
  process.
*/
uint16_t readTypeOneNum(FILE* file) {

	unsigned char first;
	fread(&first, sizeof(first), 1, file);

	unsigned char second;
	fread(&second, sizeof(second), 1, file);

	return(((uint32_t)first) << 8) | (uint32_t)second;
}

/*
 Reads three bytes from the file(incrementing the file pointer)
 and converts the charactes read to a number.
*/
uint32_t readTypeTwoAmount(FILE* file)
{
	unsigned char number[TYPE_TWO_AMOUNT_SIZE];
	fread(number, TYPE_TWO_AMOUNT_SIZE, 1, file);
	return atoi(number); // convert character array to integer
}

unsigned char readTypeOneAmount(FILE* file){
	unsigned char amount;
	fread(&amount, TYPE_ONE_AMOUNT_SIZE, 1, file);
	return amount;
}

bool isEndOfNumber(unsigned char c){
	return c == ',' || c == '\x1' || c == '\0';
}


bool notEndOfFile(FILE* file){
	char character;
	int bytesRead = fread(&character, sizeof(character), 1, file);
	if (byteRead == sizeof(character)){
		fseek(file, -1, SEEK_CUR); 
		return true;
	}
	return false;
}

unsigned char readType(FILE* file){
	char type;
	fread(&type, TYPE_SIZE, 1, file);
	return type;
}

void writeFileAndConsole(uint32_t number){
	printf("%d ", number);
	fprintf(outFile, "%d ", number);
}


/*
	Reads at maximum 5 digits from a line. Stops when a "," is seen or
	a byte denoting type is seen (\x1 or \0)
	returns a pointer to a character array containing 
	the digits for the number. Can return null.
*/
int readTypeTwoInt(FILE* file) {
	unsigned char character;
	int bytesRead = fread(&character, sizeof(character), 1, file);
	unsigned char digits[MAX_DIGITS];
	int index = 0; 
	while (bytesRead == 1 && !isEndOfNumber(character)) {
		digits[index++] = character;
		int bytesRead = fread(&character, sizeof(character), 1, file);
	} 
	return atoi(digits);
}