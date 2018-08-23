#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#define TYPE_ONE_AMOUNT_SIZE 1
#define TYPE_TWO_AMOUNT_SIZE 3
#define MAX_NUMBER_SIZE 5

/*
  Read two bytes from file and
  returns a 16 bit integer.
  Increments the file pointer in the
  process.
*/
uint16_t readTwoByteInt(FILE** file) {

	unsigned char first;
	fread(&first, sizeof(first), 1, *file);

	unsigned char second;
	fread(&second, sizeof(second), 1, *file);

	return(((uint32_t)first) << 8) | (uint32_t)second;
}

/*
 Reads three bytes from the file(incrementing the file pointer)
 and converts the charactes read to a number.
*/
uint32_t readThreeByteInt(FILE** file)
{
	unsigned char number[TYPE_TWO_AMOUNT_SIZE];
	fread(number, TYPE_TWO_AMOUNT_SIZE, 1, *file);
	return atoi(number); // convert character array to integer
}

/*
One number has been succesfully read if a "," has been read
*/
bool hasFinishedReadingNumber(unsigned char c) {
	return c == ',';
}

/*
	All numbers for a line have been read when
	the next byte that deontes a type has been seen.
	Returns whether or not character is a
*/
bool isTypeDenotingByte(unsigned char c) {
	return  c == '\x1'||c == '\0';
}

/*
	Reads at maximum 5 digits from a line. Stops when a "," is seen or
	a byte denoting type is seen (\x1 or \0)
	returns a pointer to a character array containing 
	the digits for the number. Can return null.
*/
char* readNumberAsChar(FILE** file)
{
	unsigned char character;
	int bytesRead = fread(&character, sizeof(character), 1, *file);
	if (bytesRead < sizeof(character) || hasFinishedReadingNumber(character)) { // didnt read bytes or finsihed reading a number
		return NULL;
	}

	unsigned char* number = malloc(sizeof(unsigned char) * MAX_NUMBER_SIZE);
	int index = 0; // tells us where in array we will store digit we just read.
	number[index++] = character;
	
	while (true){
		bytesRead = fread(&character, sizeof(character), 1, *file);
		if (bytesRead < sizeof(character)) {
			return number;
		}
		if (hasFinishedReadingNumber(character)) {
			break;
		}
		if (isTypeDenotingByte(character)) {
			fseek(*file, -1, SEEK_CUR); 
			/* Getting a char increments file pointer by 1.
			   The char recieved tells us if we need to stop
			   but we need to read that char again for the next iteration.
				We need to decrement by 1 to be able to read again later.
			*/
			break;
		}
		number[index++] = character;
	} 

	return number;
}

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

	while (true) {
		unsigned char type;
		int bytesRead = fread(&type, sizeof(type), 1, inFile);
		if (bytesRead < sizeof(type)) {
			break;
		}
		if (type == 0) {
			unsigned char amount;
			fread(&amount, TYPE_ONE_AMOUNT_SIZE, 1, inFile);
			printf("%d ", amount);
			fprintf(outFile, "%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				uint16_t number = readTwoByteInt(&inFile);
				printf("%d ", number);
				fprintf(outFile, "%d ", number);
			}
			uint16_t number = readTwoByteInt(&inFile);
			printf("%d\n", number);
			fprintf(outFile, "%d\n", number);
		}
		else {
			uint32_t amount = readThreeByteInt(&inFile);
			printf("%d ", amount);
			fprintf(outFile, "%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				unsigned char* number = readNumberAsChar(&inFile);
				if (number != NULL) {
					printf("%d,", atoi(number));
					fprintf(outFile, "%d,", atoi(number));
				}
				free(number);
			}
			unsigned char* number = readNumberAsChar(&inFile);
			if (number != NULL) {
				printf("%d\n", atoi(number));
				fprintf(outFile, "%d\n", atoi(number));
				free(number);
			}
		}
	}
	fclose(inFile);
	fclose(outFile);

}