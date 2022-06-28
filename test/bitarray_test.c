/*******************************************************************************
****************************** - BIT ARRAY -  **********************************
***************************** Data Structures **********************************
*
*	DESCRIPTION		Tests 
* 	NAME 			Liad Raz
*
*******************************************************************************/

#include <stdio.h> 	/* printf, putchar */
#include <stdlib.h> 	/* malloc */

#include "bitarray.h"

#define BIT_ARR_SIZE 64
#define NULL_BYTE 1

void TestBitArrSet(void);
void TestBitArrToString(void);
void TestBitArrSetOnOff(void);
void TestBitArrSetBit(void);
void TestBitArrGetVal(void);
void TestBitArrFlip(void);
void TestBitArrMirror(void);
void TestBitArrRotate(void);
void TestBitArrCount(void);


int main(void)
{
	puts("\n\t--------- Data Structures - BitArray ---------\n");

	TestBitArrSet();
	TestBitArrToString();
	TestBitArrSetOnOff();
	TestBitArrSetBit();
	TestBitArrGetVal();
	TestBitArrFlip();
	TestBitArrMirror();
	TestBitArrRotate();
	TestBitArrCount();
	
	putchar('\n');
	return 0;
}

/*******************************************************************************
******************************** Tests Impl ***********************************/

void TestBitArrSet(void)
{
	bit_array_ty bit_arr = 0xAACC2244FFCC2211;

	puts("\n\t-- BitArr Set --");
	printf("Original =>\t%lx\n", bit_arr);

	printf("SetAll =>\t%lx\n", BitArrSetAll(bit_arr));
	
	printf("ResetAll =>\t%lx\n", BitArrResetAll(bit_arr));
}

void TestBitArrToString(void)
{
	bit_array_ty bit_arr = 0xFFCC2211FFCC2211;
	/* Create buffer with thw size of 65 bytes */
	char *buffer = (char *)malloc((BIT_ARR_SIZE +  NULL_BYTE) * sizeof(char));
	
	/* memory check */
	if (NULL == buffer)
	{
		return;
	}
	
	puts("\n\t-- BitArrToString --");
	printf("Original =>\t%lx\n", bit_arr);
	buffer = BitArrToString(bit_arr, buffer);
	printf("Buffer =>\t%s\n", buffer);
	
	free(buffer);
	buffer = NULL;
	
	putchar('\n');
}

void TestBitArrSetOnOff(void)
{
	bit_array_ty bit_arr = 0xFFCC2211FFCC2211;
	size_t index = 1;
	
	puts("\n\t-- BitArrSetOnOff --");
	printf("Original =>\t%lx\n", bit_arr);
	/* ... >0< 001 0001 */
	printf("Index =>\t%lu\n", index);

	printf("Set On =>\t%lx\n", BitArrSetOn(bit_arr, index));
	/* ... >1< 001 0001 */

	printf("Set Off =>\t%lx\n", BitArrSetOff(bit_arr, index));
	/* ... >0< 001 0001 */
	
	putchar('\n');
}

void TestBitArrSetBit(void)
{
	bit_array_ty bit_arr = 0xFFCC2211FFCC2211;
	size_t index = 1;
	int state = 0;
	
	puts("\n\t-- Set Bit State --");
	printf("Original\t%lx\n", bit_arr);
	printf("Index\t%lu\n", index);
	printf("State\t%d\n", state);
	
	printf("Retrurn =>\t%lx\n", BitArrSetBit(bit_arr, index, state));
	
	putchar('\n');
}

void TestBitArrGetVal(void)
{
	bit_array_ty bit_arr = 0xFFCC2211FFCC2208;
	size_t index = 4;
	
	puts("\n\t-- Bit Value --");
	printf("Original\t%lx\n", bit_arr);
	/* ... 0000 1000 */
	printf("Index\t%lu\n", index);
	
	printf("Value is => %d", BitArrGetVal(bit_arr, index));
	
	putchar('\n');
}


void TestBitArrFlip(void)
{
	bit_array_ty bit_arr = 0xFFCC2211FFCC2208;
	size_t index = 2;
	
	puts("\n\t-- Bit Flip --");
	printf("Original\t%lx\n", bit_arr);
	/* ... 0000 1000 */
	printf("Index\t%lu\n", index);

	printf("Retrurn => %lx", BitArrFlip(bit_arr, index));
	
	putchar('\n');
}

void TestBitArrMirror(void)
{
	char buffer1[BIT_ARR_SIZE + 1] = {0};
	bit_array_ty bit_arr1 = 0xaabbccddeeff1122;
	
	char buffer2[BIT_ARR_SIZE + 1] = {0};
	bit_array_ty bit_arr2 = 0xaabbccddeeff1122;

	puts("\n\t-- Bit Mirror --");
	printf("Original %s\n %lx\n", BitArrToString(bit_arr1, buffer1), bit_arr1);
	bit_arr1 = BitArrMirror(bit_arr1);
	printf("bit_arr1 %s\n %lx\n", BitArrToString(bit_arr1, buffer1), bit_arr1);
	
	puts("\n\t-- Bit Mirror LUT --");
	printf("Original %s\n %lx\n", BitArrToString(bit_arr2, buffer2), bit_arr2);
	bit_arr2 = BitArrMirrorLUT(bit_arr2);
	printf("bit_arr2 %s\n %lx\n", BitArrToString(bit_arr2, buffer2), bit_arr2);
	
	putchar('\n');
}

void TestBitArrRotate(void)
{
	bit_array_ty bit_arrR = 0xFFCCFFEEFFAAFF22;
	bit_array_ty bit_arrL = 0xFFCCFFEEFFAAFF22;
	size_t shifts = 3;
	
	char buffer[BIT_ARR_SIZE + 1] = {0};
	
	puts("\n\t-- Bit Rotate --");
	printf("Original\n%s\n", BitArrToString(bit_arrR, buffer));
	/* 111111111111001100111111111110111011111111101010101111111100100010 */
/*	==>	Right*/
	/* 1011111111110011001111111111101110111111111010101011111111001000 */
/*	<== Left*/
	/* 111111111100110011111111111011101111111110101010111111110010001011 */
	
	puts("Bit Rotate Right");
	bit_arrR = BitArrRotR(bit_arrR, shifts);
	printf("%s\n", BitArrToString(bit_arrR, buffer));
	
	puts("Bit Rotate Left");
	bit_arrL = BitArrRotL(bit_arrL, shifts);
	printf("%s\n", BitArrToString(bit_arrL, buffer));

	putchar('\n');
}

void TestBitArrCount(void)
{
	bit_array_ty bit_arr = 0x15; 
	/* ... 1111 1111 */
	
	puts("\n\t-- Bit Count --");
	printf("Set bits On =>\t%lu\n", BitArrCountOn(bit_arr));

	printf("Bits Off =>\t%lu\n", BitArrCountOff(bit_arr));

	puts("\n\t-- Bit Count LUT --");
	printf("Set bits ON =>\t%lu\n", BitArrCountOnLUT(bit_arr));

	putchar('\n');
}





