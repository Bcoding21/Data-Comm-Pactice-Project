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

uint16_t readInt16(FILE*);

unsigned char readTypeOneAmount(FILE*);

uint32_t readTypeTwoAmount(FILE*);

unsigned char readByte(FILE*);

bool notEndOfFile(FILE*);

uint16_t readAsciiAsInt16(FILE*);

void writeFileAndConsole(int, FILE*);

bool isEndOfNumber(FILE*);


int main(int argc, char **argv) {
	/*if (argc != 3) {
	printf("Need two args. %d provided", argc - 1);
	exit(1);
	}*/

	FILE* inFile = NULL;
	//inFile = fopen(argv[1], "rb");
	fopen_s(&inFile, "file2.bin", "rb");

	FILE* outFile = NULL;
	//outFile = fopen(argv[2], "w");
	fopen_s(&outFile, "lol.txt", "w");

	if (!inFile && !outFile) {
		printf("Cannot open 1 or more files");
		exit(1);
	}

	while (notEndOfFile(inFile)) {
		unsigned char type = readByte(inFile);
		if (type == TYPE_ONE) {
			unsigned char amount = readByte(inFile);
			printf("%d ", amount);
			fprintf(outFile, "%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				uint16_t number = readInt16(inFile);
				printf("%d,", number);
				fprintf(outFile, "%d ", number);
			}
			uint16_t number = readInt16(inFile);
			printf("%d\n", number);
			fprintf(outFile, "%d\n", number);
		}
		else if (type == TYPE_TWO) {
			uint32_t amount = readTypeTwoAmount(inFile);
			printf("%d ", amount);
			fprintf(outFile, "%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				uint16_t number = readAsciiAsInt16(inFile);
				printf("%d,", number);
				fprintf(outFile, "%d ", number);
			}
			uint16_t number = readAsciiAsInt16(inFile);
			printf("%d\n", number);
			fprintf(outFile, "%d\n", number);
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
uint16_t readInt16(FILE* file) {

	unsigned char first;
	fread(&first, sizeof(first), 1, file);

	unsigned char second;
	fread(&second, sizeof(second), 1, file);

	return  ( ( (uint16_t) first ) << 8) | (uint16_t)second;
}

/*
Reads three bytes from the file(incrementing the file pointer)
and converts the charactes read to a number.
*/
uint32_t readTypeTwoAmount(FILE* file) {
	unsigned char number[TYPE_TWO_AMOUNT_SIZE];
	fread(number, TYPE_TWO_AMOUNT_SIZE, 1, file);
	return atoi(number); // convert character array to integer
}

unsigned char readTypeOneAmount(FILE* file) {
	unsigned char amount;
	fread(&amount, TYPE_ONE_AMOUNT_SIZE, 1, file);
	return amount;
}

bool isEndOfNumber(FILE* file) {
	unsigned char c;
	size_t bytesRead = fread(&c, sizeof(c), 1, file);

	if (bytesRead < 1) {
		return true;
	}

	if (c == '\0' || c == '\x1') {
		fseek(file, -1, SEEK_CUR);
		return true;
	}

	if (c == ',') {
		return true;
	}

	fseek(file, -1, SEEK_CUR);
	return false;
}


bool notEndOfFile(FILE* file) {
	char character;
	int bytesRead = fread(&character, sizeof(character), 1, file);
	if (bytesRead == sizeof(character)) {
		fseek(file, -1, SEEK_CUR);
		return true;
	}
	return false;
}

unsigned char readByte(FILE* file) {
	char byte;
	fread(&byte, TYPE_SIZE, 1, file);
	return byte;
}

void writeFileAndConsole(int number, FILE* file) {
	printf("%d ", number);
	fprintf(file, "%d ", number);
}


/*
Reads at maximum 5 digits from a line. Stops when a "," is seen or
a byte denoting type is seen (\x1 or \0)
returns a pointer to a character array containing
the digits for the number. Can return null.
*/
uint16_t readAsciiAsInt16(FILE* file) {
	unsigned char digits[MAX_DIGITS];
	int index = 0;
	while (!isEndOfNumber(file)) {
		unsigned char character;
		fread(&character, sizeof(character), 1, file);
		digits[index++] = character;
	}
	return atoi(digits);
}