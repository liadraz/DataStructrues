
/*******************************************************************************
****************************** - HASH_TABLE - **********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Tests Hash Table container
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdio.h>		/* printf, puts, putchar, size_t */
#include <stdlib.h>		/* malloc, free */
#include <string.h>		/* strncpy */
#include <sys/stat.h>		/* stat, fstat */
#include <sys/mman.h>		/* mmap */
#include <fcntl.h>		/* open */
#include <unistd.h>		/* close */

#include "utilities.h"
#include "hash_table.h"

#define NULL_BYTE		1

typedef struct stat stat_ty;

void TestHashTableCreate(void);
void TestHashTableInsert(void);
void TestHashTableRemove(void);
void TestHashHashTableCount(void);

/* CallBack Functions */
size_t HashFunc(const void *word_, const void *param_);
int IsSameKey(const void *key1, const void *key2);
static void TestHashFunc(void);

/* Load Dictionary */
static char **LoadDictionary(void);
static char **CopyWords(void *map_);
static size_t WordLength(char *word_);

/* Side Functions */
static void PrintTestStatusIMP(size_t tcounter_, size_t num_of_tests_, char *test_name_);


int main(void)
{
	PRINT_MSG(\n\t--- Tests hash_table ---\n);
	
	TestHashTableCreate();
 	TestHashTableInsert();
 	TestHashTableRemove();
 	TestHashHashTableCount();
	
/*	TestHashFunc();*/
/*	words = LoadDictionary();*/
	
	NEW_LINE;
	NEW_LINE;
	return 0;
}


void TestHashTableCreate(void)
{
	ht_ty *hash_table = NULL;
	void *param = NULL;
	size_t hash_size = 11;

	size_t tcount = 0;
	
	hash_table = HashTableCreate(HashFunc, hash_size, IsSameKey, param);
	
	if (hash_table != NULL)
	{ ++tcount; }
	
	PrintTestStatusIMP(tcount, 1, "Create");

	HashTableDestroy(hash_table);
}

void TestHashTableInsert(void)
{
	ht_ty *hash_table = NULL;
	size_t hash_size = 29;
	void *param = NULL;
	
	char *to_add1 = "Biscuit";
	char *to_add2 = "Liad";
	char *to_add3 = "Ori";

	ht_itr_ty ret_itr = {NULL};
	size_t tcount = 0;
	
	hash_table = HashTableCreate(HashFunc, hash_size, IsSameKey, param);
	if (hash_table == NULL)	
	{
	 return; 
	}
	
	/* Insertion */
	ret_itr = HashTableInsert(hash_table, (void *)to_add1);
	ret_itr = HashTableInsert(hash_table, (void *)to_add2);
	ret_itr = HashTableInsert(hash_table, (void *)to_add3);

	if (hash_table == ret_itr.hash_table)
	{ ++tcount; }
 
	if (to_add3 == HashTableGetData(ret_itr))
	{ ++tcount; }
 
	PrintTestStatusIMP(tcount, 2, "Insert");
	
	HashTableDestroy(hash_table);
}


void TestHashTableRemove(void)
{
	ht_ty *hash_table = NULL;
	size_t hash_size = 29;
	void *param = NULL;

	char *to_add1 = "Biscuit";
	char *to_add2 = "Liad";

	ht_itr_ty to_remove1 = {NULL};
	ht_itr_ty to_remove2 = {NULL};
	size_t tcount = 0;

	hash_table = HashTableCreate(HashFunc, hash_size, IsSameKey, param);
	if (hash_table == NULL)	
	{
	 return; 
	}

	to_remove1 = HashTableInsert(hash_table, (void *)to_add1);
	to_remove2 = HashTableInsert(hash_table, (void *)to_add2);

	HashTableRemove(to_remove1);
	HashTableRemove(to_remove2);

	PrintTestStatusIMP(tcount, 2, "Remove");
	
	HashTableDestroy(hash_table);
}

void TestHashHashTableCount(void)
{
	ht_ty *hash_table = NULL;
	size_t hash_size = 29;
	void *param = NULL;

	char *to_add1 = "Biscuit";
	char *to_add2 = "Liad";
	char *to_add3 = "Ori";
	char *to_add4 = "Omri";

	ht_itr_ty itr = {NULL};
	size_t tcount = 0;

	hash_table = HashTableCreate(HashFunc, hash_size, IsSameKey, param);
	if (hash_table == NULL)	
	{
	 return; 
	}

	if (0 == HashTableCount(hash_table))
	{ ++tcount; }

	itr = HashTableInsert(hash_table, (void *)to_add1);
	itr = HashTableInsert(hash_table, (void *)to_add2);

	if (2 == HashTableCount(hash_table))
	{ ++tcount; }


	itr = HashTableInsert(hash_table, (void *)to_add3);
	itr = HashTableInsert(hash_table, (void *)to_add4);

	HashTableRemove(itr);

	if (3 == HashTableCount(hash_table))
	{ ++tcount; }

	PrintTestStatusIMP(tcount, 3, "Count");
	
	HashTableDestroy(hash_table);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CallBack Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
size_t HashFunc(const void *word_, const void *param_)
{
    size_t hash_number = 0;
	const char *str = word_;
	
    while ('\0' != *str)
    {
        hash_number = *str + (hash_number << 6) + (hash_number << 16) - hash_number;
        
        ++str;
    }
    
    UNUSED(param_);
    return hash_number;
}


int IsSameKey(const void *key1, const void *key2)
{
	UNUSED(key1);
	UNUSED(key2);
	return 1;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Auxilary Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void TestHashFunc(void)
{
	char *str[] = {"liad", "Yarden", "Amram", "Yaara", "Omri"};
	void *param = NULL;
	size_t hash_value = 0;
	size_t i = 0;
	
	NEW_LINE;
	
	for (i = 0; i < 5; i++)
	{
		hash_value = HashFunc((void *)str[i], param);
	}

	printf("HASH_VALUE %lu\n", hash_value);
}

static void PrintTestStatusIMP(size_t tcounter_, size_t num_of_tests_, char *test_name_)
{
	if (tcounter_ == num_of_tests_)
	{
		GREEN;
		printf("\tTest %s: SUCCESS", test_name_);
		DEFAULT;
	}
	else
	{
		RED;
		printf("\tTest %s: FAILED", test_name_);
		DEFAULT;
	}

	NEW_LINE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Load Dict Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static char **LoadDictionary(void)
{	
/*	FILE *dict_fd = NULL;*/

	stat_ty file_info = {0};
	char *dict_file_path = "/usr/share/dict/american-english";
	char **words_buff = NULL;
	void *dict_map = NULL;
	int dict_fd = 0;
	int ret_err = 0;
	
	/* Get a file descriptor of the curent file */
	dict_fd = open(dict_file_path ,O_RDONLY);

	if (dict_fd < 0)
	{
		fputs("LoadDictionary: Error Opening file for reading", stderr);
		return NULL;
	}
	
	/* Get statistics about the file using stat struct and function */
	ret_err = fstat(dict_fd, &file_info);


	if (ret_err < 0)
	{
		fputs("LoadDictionary: Error getting the file size", stderr);
		return NULL;
	}

	if (0 == file_info.st_size)
	{
		fputs("LoadDictionary: Error, File is Empty", stderr);
		return NULL;
	}


	/* Get pointer to the desired file, In read mode*/
	dict_map = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, dict_fd, 0);
	if (dict_map == MAP_FAILED)
	{
		close(dict_fd);
		fputs("LoadDictionary: Error Mapping the file", stderr);
		return NULL;
	}
	
	/* Copy each word in the dictionary file to strings buffer */
	words_buff = CopyWords(dict_map);

	/* Close the file descriptor */
	ret_err = close(dict_fd);
	if (ret_err < 0)
	{
		fputs("LoadDictionary: Error Closing file", stderr);
		return NULL;
	}
	
	/* Unmap pointer to file */
	ret_err = munmap(dict_map, file_info.st_size);
	if (ret_err != 0)
	{
		fputs("LoadDictionary: Error UnMapping the file", stderr);
		return NULL;
	}
	
	return words_buff;
}


static char **CopyWords(void *map_)
{
	char **dict_words = NULL;
	char **begin_dict_words = NULL;
	char *word = NULL;
	char *runner = (char *)map_;
	size_t word_len = 0;
	size_t i = 0;
	
	/* Allocate buffer the size of words in file */
	/* TODO size of array is size of words */
	dict_words = (char **)malloc(102401);
	if (NULL == dict_words)
	{
		return NULL;
	}
	
	begin_dict_words = dict_words;
	
	/* Run until the end of the file */	
	for (i = 0; i < 102401; ++i)
	{
		word_len = WordLength(runner); 
		
		/* Allocate memory for word */
		word = (char *)malloc(word_len + NULL_BYTE);
		if (NULL == word)
		{
			return NULL;
		}
		
		word = strncpy(word, runner, (word_len + NULL_BYTE));
		
		/* move runner to next word in dict file */
		runner += (word_len + 1);
		
		/* move to the next element in the array to store a new word */
		++dict_words;
	}
	
	return begin_dict_words;
}

static size_t WordLength(char *word_)
{
	size_t word_len = 0;
	
	while (*word_ != '\n' )
	{
		++word_len;
		++word_;
	}
	
	return word_len;
}






















