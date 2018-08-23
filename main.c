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

	char first;
	fread(&first, sizeof(first), 1, *file);

	char second;
	fread(&second, sizeof(second), 1, *file);

	return((uint16_t)first << 8) | (uint16_t)second;
}

/*
 Reads three bytes from the file(incrementing the file pointer)
 and converts the charactes read to a number.
*/
uint32_t readThreeByteInt(FILE** file)
{
	char number[TYPE_TWO_AMOUNT_SIZE];
	fread(number, TYPE_TWO_AMOUNT_SIZE, 1, *file);
	return atoi(number);
}


bool hasFinishedReadingNumber(char c) {
	return c == ',';
}

bool hasFinishedReaddingAll(char c) {
	return  c == '\x1'||
		c == '\0';
}
char* readNumberAsChar(FILE** file)
{
	char character;
	int bytesRead = fread(&character, sizeof(character), 1, *file);
	if (bytesRead < sizeof(character) || hasFinishedReadingNumber(character)) {
		return NULL;
	}

	char* number = malloc(sizeof(char) * MAX_NUMBER_SIZE);
	int index = 0;
	number[index++] = character;
	
	while (true){
		bytesRead = fread(&character, sizeof(character), 1, *file);
		if (bytesRead < sizeof(character)) {
			return number;
		}
		if (hasFinishedReadingNumber(character)) {
			break;
		}
		if (hasFinishedReaddingAll(character)) {
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

int main() {
	
	char* fileName = "file2.bin";
	FILE* file;
	fopen_s(&file, fileName, "rb");

	while (true) {
		char type;
		int bytesRead = fread(&type, sizeof(type), 1, file);
		if (bytesRead < sizeof(type)) {
			break;
		}
		if (type == 0) {
			char amount;
			fread(&amount, TYPE_ONE_AMOUNT_SIZE, 1, file);
			printf("%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				uint16_t number = readTwoByteInt(&file);
				printf("%d ", number);
			}
			uint16_t number = readTwoByteInt(&file);
			printf("%d\n", number);
		}
		else {
			uint32_t amount = readThreeByteInt(&file);
			printf("%d ", amount);
			for (int i = 0; i < amount - 1; i++) {
				char* number = readNumberAsChar(&file);
				if (number != NULL) {
					printf("%d, ", atoi(number));
				}
				free(number);
			}
			char* number = readNumberAsChar(&file);
			if (number != NULL) {
				printf("%d\n", atoi(number));
				free(number);
			}
		}
	}
	fclose(file);

	char a[39];
	gets(a);

}