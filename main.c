#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define TYPE_ONE_AMOUNT_SIZE 1
#define TYPE_TWO_AMOUNT_SIZE 3
#define MAX_NUMBER_SIZE 5


uint16_t readTwoByteInt(FILE** file) {

	char first;
	fread(&first, sizeof(first), 1, *file);

	char second;
	fread(&second, sizeof(second), 1, *file);

	return((uint16_t)first << 8) | (uint16_t)second;
}

uint32_t readThreeByteInt(FILE** file)
{
	char number[TYPE_ONE_AMOUNT_SIZE];
	fread(number, TYPE_TWO_AMOUNT_SIZE, 1, file);
	return atoi(number);
}

char* readNumber(FILE** file)
{
	char number[MAX_NUMBER_SIZE];
	char digit;
	int index = 0;
	do {
		fread(&(digit) + index++, sizeof(digit), 1, *file);
	} while (digit != ",");
	return atoi(number);
}

int main() {

	
	char* fileName = "file1.bin";
	FILE* file;
	fopen_s(&file, fileName, "rb");

	char g[50];
	fread(g, sizeof(g), 1, file);

	char type;
	fread(&type, sizeof(type), 1, file);

	if (type == 0) {
		char amount;
		fread(&amount, TYPE_ONE_AMOUNT_SIZE, 1, file);
		printf("%d ", amount);
		for (int i = 0; i < amount - 1; i++) {
			uint16_t number = readTwoByteInt(&file);
			printf("%d," , number);
		}
		uint16_t number = readTwoByteInt(&file);
		printf("%d\n", number);
	}
	else {
		uint32_t amount = readThreeByteInt(&file);
		printf("%d ", amount);
		for (int i = 0; i < amount - 1; i++) {
			uint16_t number = readNumber(&file);
			printf("%d, ", number);
		}

	}
	fclose(file);

	char a[39];
	gets(a);

}