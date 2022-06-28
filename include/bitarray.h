/*******************************************************************************
****************************** - BIT ARRAY -  **********************************
***************************** Data Structures **********************************
*
* Description: 	BitArray API
* Name: 		Liad Raz
* Version: 		1.0 ready for review 
*
*******************************************************************************/

#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>			/* size_t */

typedef size_t bit_array_ty;


/******************************************************************************
* Function Description: Sets all bits to 1.	       
* Arguments: The bit array itself by value.
* Return value: The changed version of the bit array itself by value.
* Notes: 

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrSetAll(bit_array_ty bit_arr);


/******************************************************************************
* Function Description: Sets all bits to 0.	       
* Arguments: The bit array itself by value.
* Return value: The changed version of the bit array itself by value.
* Notes: 

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrResetAll(bit_array_ty bit_arr);


/******************************************************************************
* Function Description: BitArrToString composes a string representation of the array.	       
* Arguments: The bit array itself by value, and a buffer by reference.
* Return value: A pointer to the same buffer.
* Notes: Undefined behavior for a buffer smaller than 65 bytes.

* Time Complexity: NA
*******************************************************************************/
char *BitArrToString(const bit_array_ty bit_arr, char *buffer);


/******************************************************************************
* Function Description: BitArrSetOn sets the bit at the given index to 1.	       
* Arguments: The bit array itself by value, and an index.
* Return value: The changed version of the bit array itself by value.
* Notes: Undefined behavior for out of range indeces.

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrSetOn(bit_array_ty bit_arr, size_t index);


/******************************************************************************
* Function Description: BitArrSetOff sets the bit at the given index to 0.	       
* Arguments: The bit array itself by value, and an index.
* Return value: The changed version of the bit array itself by value.
* Notes: Undefined behavior for out of range indeces.

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrSetOff(bit_array_ty bit_arr, size_t index);


/******************************************************************************
* Function Description: BitArrSetBit sets the bit at the given index to the given state.	       
* Arguments: The bit array itself by value, an index, and a state.
* Return value: The changed version of the bit array itself by value.
* Notes: Undefined behavior for out of range indeces, and for non binary states.

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrSetBit(bit_array_ty bit_arr, size_t index, int state);


/******************************************************************************
* Function Description: This function returns the given bit index value
* Arguments: bit_arr = array of bits, index = bit's place in the array
* Return value: 0 or 1 according to the value in the bit index
* Notes: index must be between 0 to 63

* Time Complexity: NA
*******************************************************************************/
int BitArrGetVal(bit_array_ty bit_arr, size_t index);


/******************************************************************************
* Function Description: Switching state of specific bit, 0 to 1 or 1 to 0.		       
* Arguments: bit_arr - Variable of size_t that represent array of bitis.
			 index - The index of the bit for change.
* Return value: Array of the type bit_array_ty with the bit in index flip.
* Notes: 

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrFlip(bit_array_ty bit_arr, size_t index);


/******************************************************************************
* Function Description: The function creates a mirror image of bit_arr 
				and returns it.

* Arguments: 		bit_array_ty bit_arr - a bit array of 64 bits
* Return value:		The function returns a bit_array_ty bit array,
				which is the mirror image of bit_arr.
* Notes:			/

* Time Complexity:	NA
*******************************************************************************/
bit_array_ty BitArrMirror(bit_array_ty bit_arr);
bit_array_ty BitArrMirrorLUT(bit_array_ty bit_arr);


/******************************************************************************
* Function Description: shifts bits to the right, number of steps according to 
    the amount parameter, the bits that "fall off" to the right are added to the 
    left, simulating a circular motion.
* Arguments: bit_array_ty bit_array - a variable of size_t that represents the 
    bit array. 
    size_t shifts - the amount of shifts in the rotation.
* Return value:  The changed version of the bit array itself by value.
* Notes: 

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrRotR(bit_array_ty bit_arr, size_t shifts);


/******************************************************************************
* Function Description: shifts bits to the left, number of steps according to 
    the amount parameter, the bits that "fall off" to the left are added to the 
    right, simulating a circular motion.
* Arguments: bit_array_ty bit_array - a variable of size_t that represents the 
    bit array. 
    size_t shifts - the amount of shifts in the rotation.
* Return value:  The changed version of the bit array itself by value.
* Notes: 

* Time Complexity: NA
*******************************************************************************/
bit_array_ty BitArrRotL(bit_array_ty bit_arr, size_t shifts);


/******************************************************************************
* Function Description: The function count_on counts the number of lit bits
* Arguments: bit_arr - varible of size_t that represent a array of bit
* Return value: the number of lit bit
*
*******************************************************************************/
size_t BitArrCountOn(bit_array_ty bit_arr);
size_t BitArrCountOnLUT(bit_array_ty bit_arr);

/******************************************************************************
* Function Description: The function count_off counts the number of turned off bits
* Arguments: bit_arr - varible of size_t that represent a array of bit
* Return value: the number of turned off bit

*******************************************************************************/
size_t BitArrCountOff(bit_array_ty bit_arr);


#endif /* __BIT_ARRAY_H__ */
