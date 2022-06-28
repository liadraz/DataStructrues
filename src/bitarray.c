/*******************************************************************************
****************************** - BIT ARRAY -  **********************************
***************************** Data Structures **********************************
*
* Name: 		Liad Raz
* Compile		gc src/bitarray.c test/bitarray_test.c -I ./include
*
*******************************************************************************/

#include <stdio.h>
#include <assert.h> /* assert */

#include "utilities.h"
#include "bitarray.h"

#define BYTE 8
#define BIT_ARR_SIZE 64
#define NIBBLE_MAX_VALUE 16
#define BYTE_MAX_VALUE 256

#define MASK_ONE (bit_array_ty)1
#define MASK_ZERO (bit_array_ty)0
#define MASK_BYTE (bit_array_ty)0xFF

#define UNUSED(x) (void)(x)

static void InitMirrorLUT(unsigned char *mirror_arr);
static unsigned char MirrorOneByte(unsigned char byte);

static size_t CountSetBits(unsigned char byte);
static void InitCountLUT(size_t *count_set_LUT);

static size_t CreateMaskInIndex(size_t index);

/*******************************************************************************
******************************* BitArrSetAll **********************************/
bit_array_ty BitArrSetAll(bit_array_ty bit_arr)
{
	UNUSED(bit_arr);
	return ~(bit_array_ty)0;
}


/*******************************************************************************
****************************** BitArrResetAll *********************************/
bit_array_ty BitArrResetAll(bit_array_ty bit_arr)
{
	UNUSED(bit_arr);
	return 0;
}


/*******************************************************************************
****************************** BitArrToString *********************************/
char *BitArrToString(const bit_array_ty bit_arr, char *buffer)
{
	bit_array_ty mask_one = 0;
	char *buffer_runner = buffer;
	
	assert (buffer != NULL);

	/* Create a mask that the leftmost bit is set */
	mask_one = CreateMaskInIndex(BIT_ARR_SIZE);
	
	while (0 < mask_one)
	{
		*buffer_runner = (mask_one & bit_arr) ? '1' : '0';
		mask_one >>= 1;
		
		++buffer_runner;
	}
	
	/* Add null to last byte */
	*buffer_runner = '\0';
	
	return buffer;
}


/*******************************************************************************
****************************** BitArrSetOn ************************************/
bit_array_ty BitArrSetOn(bit_array_ty bit_arr, size_t index)
{
	bit_array_ty mask_indexed = CreateMaskInIndex(index);
	
	bit_arr = (mask_indexed | bit_arr);
	
	return bit_arr;
}


/*******************************************************************************
****************************** BitArrSetOff ***********************************/
bit_array_ty BitArrSetOff(bit_array_ty bit_arr, size_t index)
{
	bit_array_ty mask_indexed = CreateMaskInIndex(index);
	
	/* Negate the mask to get 0 at the index position */
	mask_indexed = ~(mask_indexed);
	
	bit_arr = (mask_indexed & bit_arr);
	
	return bit_arr;
}


/*******************************************************************************
****************************** BitArrSetBit ***********************************/
bit_array_ty BitArrSetBit(bit_array_ty bit_arr, size_t index, int state)
{
	assert (index <= BIT_ARR_SIZE);
/*	assert (state != 1 || state != 0);*/

	/* When state is 0 turn the bit OFF, and when state is set thern it ON */
	if (0 == state)
	{
		bit_arr = BitArrSetOff(bit_arr, index);
	}
	else
	{
		bit_arr = BitArrSetOn(bit_arr, index);
	}
	
	return bit_arr;
}


/*******************************************************************************
****************************** BitArrGetVal ***********************************/
int BitArrGetVal(bit_array_ty bit_arr, size_t index)
{
	bit_array_ty mask_indexed = CreateMaskInIndex(index);
	
	return (bit_arr & mask_indexed) ? 1 : 0;
}


/*******************************************************************************
**************************** BitArrFlip *********************************/
bit_array_ty BitArrFlip(bit_array_ty bit_arr, size_t index)
{
	bit_array_ty mask_indexed = CreateMaskInIndex(index);
	
	/* Xor operator will flip the bit when is different from mask */
	return (bit_arr ^ mask_indexed);
}


/*******************************************************************************
**************************** BitArrMirror *************************************/
bit_array_ty BitArrMirror(bit_array_ty bit_arr)
{
	bit_array_ty shifts_num = BIT_ARR_SIZE - 1;
	bit_array_ty curr_bit_value = MASK_ZERO;
	bit_array_ty reversed_num = 0;
	
	while (bit_arr > 0)
	{
		/* Get right Most bit in bit_arr and shift it left shifts_num times */
		curr_bit_value = (bit_arr & MASK_ONE) << shifts_num;
		
		/* Add the current bit to the result num */
		reversed_num |= curr_bit_value;
		
		--shifts_num;
		bit_arr >>= 1;
	}
	
	return reversed_num;
}


/*******************************************************************************
**************************** BitArrMirror LUT *********************************/
bit_array_ty BitArrMirrorLUT(bit_array_ty bit_arr)
{
	static bool_ty is_init_LUT = FALSE;
	static unsigned char mirror_arr[BYTE_MAX_VALUE] = {0};
	
	bit_array_ty shift_bytes = BIT_ARR_SIZE - BYTE;
	bit_array_ty curr_byte_mirrored = MASK_ZERO;
	bit_array_ty result_bit_arr = 0;

	/* Initialize LUT only once */
	if (FALSE == is_init_LUT)
	{
		InitMirrorLUT(mirror_arr);
		is_init_LUT = TRUE;
	}
	
	/* mirror each byte in the bit_arr and assign it to result_bit_arr */
	while (bit_arr > 0)
	{
		/* Assign the mirrored masked byte to the a temporary */
		curr_byte_mirrored = (bit_array_ty)mirror_arr[(unsigned char)bit_arr];
		
		/* Shift the byte to the left position shift_bytes times */
		curr_byte_mirrored <<= shift_bytes;
		
		/* mask the byte with the result number */
		result_bit_arr |= curr_byte_mirrored;
		
		/* Shift the original bit_arr to be masked with another BYTE */
		bit_arr >>= BYTE;
		shift_bytes -= BYTE;
	}
	
	return result_bit_arr;
}

static void InitMirrorLUT(unsigned char *mirror_arr)
{
	size_t index = 0;
	
	while (index < BYTE_MAX_VALUE)
	{
		*mirror_arr = MirrorOneByte((unsigned char)index);
		
		++mirror_arr;
		++index;
	}
}

/* Mirror 8 bits using Hamming Weight method */
static unsigned char MirrorOneByte(unsigned char byte) 
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4; /* 0xF0 11110000 0x0F 00001111 */
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2; /* 0xCC 11001100 0x33 00110011*/
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1; /* 0xAA 10101010 0x55 01010101*/
    
    return byte;
}


/*******************************************************************************
***************************** BitArrRotR **************************************/
bit_array_ty BitArrRotR(bit_array_ty bit_arr, size_t shifts)
{
	shifts %= BIT_ARR_SIZE;
	
	return bit_arr >> shifts | bit_arr << (BIT_ARR_SIZE - shifts);
}


/*******************************************************************************
***************************** BitArrRotL **************************************/
bit_array_ty BitArrRotL(bit_array_ty bit_arr, size_t shifts)
{
	shifts %= BIT_ARR_SIZE;
	
	return bit_arr << shifts | bit_arr >> (BIT_ARR_SIZE - shifts);
}


/*******************************************************************************
***************************** BitArrCountOn ***********************************/
size_t BitArrCountOn(bit_array_ty bit_arr)
{
	size_t counter_on = 0;
	
	while (bit_arr > 0)
	{
		if (0 != (MASK_ONE & bit_arr))
		{
			++counter_on;
		}
		bit_arr >>= 1;
	}
	
	return counter_on;
}


/*******************************************************************************
***************************** BitArrCountOFF **********************************/
size_t BitArrCountOff(bit_array_ty bit_arr)
{
	return BIT_ARR_SIZE - BitArrCountOn(bit_arr);
}


/*******************************************************************************
**************************** BitArrCountOn LUT ********************************/
size_t BitArrCountOnLUT(bit_array_ty bit_arr)
{
	static bool_ty is_init_LUT = FALSE;
	static size_t count_set_LUT[BYTE_MAX_VALUE] = {0};
	size_t count_set_bits = 0;
	
	/* Initialize LUT only once */
	if (FALSE == is_init_LUT)
	{
		InitCountLUT(count_set_LUT);
		is_init_LUT = TRUE;
	}
	
	/* Count each byte in the bit_arr */
	while (bit_arr > 0)
	{
		/* get the number of set bits in a byte from the LUT */
		count_set_bits += count_set_LUT[(unsigned char)bit_arr];
		
		/* Shift the original bit_arr BYTE times right*/
		bit_arr >>= BYTE;
	}
	/* iterate each Byte and get the count Set bits number using the LUT */
	
	return count_set_bits;
}

static size_t CountSetBits(unsigned char byte)
{
	size_t counter = 0;
	
	while (byte > 0)
	{
		if (!!(MASK_ONE & byte))
		{
			++counter;
		}
		byte >>= 1;
	}
	
	return counter;
}

static void InitCountLUT(size_t *count_set_LUT)
{
	size_t index = 0;
	
	while (index < BYTE_MAX_VALUE)
	{
		*count_set_LUT = CountSetBits(index);
		
		++count_set_LUT;
		++index;
	}
}


/*******************************************************************************
**************************** Utilities Function *******************************/

/* Create a mask that one bit is set at the given index position */
static size_t CreateMaskInIndex(size_t index)
{
	if (0 == index)
	{
		return MASK_ZERO;
	}
	
	return MASK_ONE << (index - 1);
}




/*******************************************************************************
***************************** - PSUEDOCODE -  **********************************
********************************************************************************
(1)
	return a mask that all bits are 1

--------------------------------------------------------------------------------
(2)
	return a mask that all bits are 0
--------------------------------------------------------------------------------
(3)	
	iterate each bit in the bit_array
		if equals to 1 
			char will be 1
		else
			char will be zero
			
		append char to the buffer
	
--------------------------------------------------------------------------------
	MIRROR

	bit_array_ty curr_bit_value = MASK_ZERO;
	bit_array_ty reversed_bit_arr = MASK_ZERO;
	
	bit_array_ty mask_one = MASK_ONE;
	
	size_t i = 0;
	
	while (i < BIT_ARR_SIZE)
	{
		curr_bit_value = (bit_arr & mask_one);
		
		reversed_bit_arr |= curr_bit_value;

		reversed_bit_arr <<= 1;
		mask_one <<= 1;
		
		printf("%lu\t", i);
		printf("%lx\t", reversed_bit_arr);
		
		++i;
	}
	
	 Apply the last digit to reverse number 
	curr_bit_value = (bit_arr & MASK_ONE );
	reversed_bit_arr |= curr_bit_value;
	
	return reversed_bit_arr;
	
*******************************************************************************/
