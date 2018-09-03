#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#define TYPE_SIZE 1
#define TYPE_ONE 0
#define TYPE_TWO 1
#define TYPE_TWO_AMOUNT_SIZE 3
#define MAX_DIGITS 5

uint16_t readBytesAsInt16(FILE*);

uint32_t readTypeTwoAmount(FILE*);

uint16_t readAsciiAsInt16(FILE*);

bool finishedReadingNumber(FILE*);

bool isType(char);


int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Need two args. %d provided", argc - 1);
		return EXIT_FAILURE;
	}

	FILE* inFile = NULL;
	//inFile = fopen(argv[1], "rb");
	fopen_s(&inFile, argv[1], "rb");

	FILE* outFile = NULL;
	//outFile = fopen(argv[2], "w");
	fopen_s(&outFile, argv[2], "w");

	if (!inFile || !outFile) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}

	while (!feof(inFile)) {
		unsigned char type = fgetc(inFile);
		if (type == TYPE_ONE) {
			unsigned char amount = fgetc(inFile);
			printf("%d ", amount);
			fprintf(outFile, "%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				uint16_t number = readBytesAsInt16(inFile);
				printf("%d,", number);
				fprintf(outFile, "%d ", number);
			}
			uint16_t number = readBytesAsInt16(inFile);
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
uint16_t readBytesAsInt16(FILE* file) {
	unsigned char first = fgetc(file);
	unsigned char second = fgetc(file);
	return  ( ( (uint16_t) first ) << 8) | (uint16_t)second;
}

/*
Reads three bytes from the file(incrementing the file pointer)
and converts the charactes read to a number.
*/
uint32_t readTypeTwoAmount(FILE* file) {
	unsigned char number[TYPE_TWO_AMOUNT_SIZE];
	fread(number, sizeof(number[0]), TYPE_TWO_AMOUNT_SIZE,  file);
	return atoi(number); // convert character array to integer
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
	while (!finishedReadingNumber(file)) {
		digits[index++] = fgetc(file);
	}
	return atoi(digits);
}

bool finishedReadingNumber(FILE* file) {

	unsigned int nextByte = fgetc(file);

	if (nextByte == EOF) {
		return true;
	}

	if (isdigit(nextByte)) {
		fseek(file, -1, SEEK_CUR);
		return false;
	}

	if (isType(nextByte)) {
		fseek(file, -1, SEEK_CUR);
		return true;
	}

	return nextByte == ',';
}


bool isType(char c) {
	return c == '\0' || c == '\x1';
}