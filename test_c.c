#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"
#include "heaplib_naive.c"

#define SUCCESS 1
#define FAIL 0

// when testing for correctness make heap nice and big
// don't want bloated meta data to be the problem
#define DECENT_HEAP_SIZE 256

int num_tests = 0;

/*checking if resize coppies values from the old block to the new block*/

int resize_copies_values() {

	char heap[DECENT_HEAP_SIZE]; 
	char *letters, *new_letters;
	
	num_tests++; // begin each test by incrementing this counter
	
	hl_init(heap, DECENT_HEAP_SIZE);
	letters = (char *) hl_alloc(heap, 6 * sizeof(char));
	
	letters[0] = 'h';
	letters[1] = 'e';
	letters[2] = 'l';
	letters[3] = 'l';
	letters[4] = 'o';
	letters[5] = '\0';
	
	new_letters = hl_resize(heap, letters, 20 * sizeof(char));
	
	printf("%d) Resize copies values?", num_tests); 
	if (!strcmp(letters, new_letters)) {
		printf("\t\t\tPASS\n");
		return SUCCESS;
	}
	
	printf("\t\t\tFAIL\n"); 
	return FAIL;
	
}

/*check that tresize behave like alloc when blockptr is 0. Comaprig the string size of
the initial block and the new block returns >0*/
int resize_blockpr_is_0() {

	char heap[DECENT_HEAP_SIZE]; 
	char *letters, *new_letters;
	
	num_tests++; // begin each test by incrementing this counter
	
	hl_init(heap, DECENT_HEAP_SIZE);
	letters = (char *) hl_alloc(heap, 6 * sizeof(char));
	
	letters[0] = 'h';
	letters[1] = 'e';
	letters[2] = 'l';
	letters[3] = 'l';
	letters[4] = 'o';
	letters[5] = '\0';
	
	new_letters = hl_resize(heap, 0, 20 * sizeof(char));
	
	printf("%d) Resize with new address 0?", num_tests); 
	if (strcmp(letters, new_letters) > 0) {
		printf("\t\t\tPASS\n");
		return SUCCESS;
	}
	
	printf("\t\t\tFAIL\n"); 
	return FAIL;
	
}

/*check if hl_resize fails when the 
heapsize is less than the header size*/

int init_failed_heapsize(){

	char array[DECENT_HEAP_SIZE]; 

	num_tests++; // begin each test by incrementing this counter

	unsigned int header_size = 1;//1 is definately less than the header size

	int check = hl_init(&array, header_size);
	printf("%d) hl_init fails on heapsize less than header size?", num_tests); 
	if (check == 0) {
		printf("\t\t\tPASS\n");
		return SUCCESS;
	}
	
	printf("\t\t\tFAIL\n"); 
	return FAIL;
}
/*check if hl_allock fails when the blocksize is bigger than 
the available free bytes*/
int alloc_failed_block_size(){
	char array[DECENT_HEAP_SIZE]; 
	//void *block;

	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);


	
	int *block = (int *)hl_alloc(&array, DECENT_HEAP_SIZE);
	//printf("%p\n", block);
	//unsigned int check = (unsigned int)block;

	printf("%d) hl_alloc fails on blocksize bigger than bytes free?", num_tests); 
	if (block == 0) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
	

}

/*check if hl_resize fails when the new_blocksize is bigger 
that the available free bytes*/

int resize_failed_newBlockSize(){
	char array[DECENT_HEAP_SIZE]; 
	//void *block;

	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);


	
	char *block = (char *)hl_alloc(&array, 6*sizeof(char));

	int *new_block = (int *)hl_resize(&array,block, DECENT_HEAP_SIZE);
	//printf("%p\n", block);
	//unsigned int check = (unsigned int)block;

	printf("%d) hl_resize fails on new_blocksize bigger than bytes free?", num_tests); 
	if (new_block == 0) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
	


}

/*check if hl_resize fails when the new blockpr is not 8 byte alined. If blockptr is not 
8 byte aligned, it returns a 0*/
int resize_failed_blockptr(){

	char array[DECENT_HEAP_SIZE]; 
	//void *block;

	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);


	
	char *block = (char *)hl_alloc(&array, 6*sizeof(char));

	int *new_block = (int *)hl_resize(&array,block + 1 , 7*sizeof(char));
	//printf("%p\n", block);
	//unsigned int check = (unsigned int)block;

	printf("%d) hl_resize fails on blockptr not aligned?", num_tests); 
	if (new_block == 0) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
}
/* Note: this test may or may not be robust. Take a look and determine
 * whether it thoroughly tests what it is trying to test.
 */

 /*check if the returned pointer after resize is correclty aligned given a new blocksize 
 that is not 8 byte aligned*/
int resize_ptr_aligned(){
	char array[DECENT_HEAP_SIZE]; 
	void *block;
	void *block2;
	bool aligned_f = false;

	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);
	
	block = hl_alloc(&array, 11);
	block2 = hl_resize(&array, block, 17);
	aligned_f = !((unsigned long)block2 & (ALIGNMENT - 1));

	printf("%d) hl_resize return ptr has correct alignment?", num_tests); 
	if (aligned_f) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;


}

/*check if the pointer returned by hl_alloc is correctly aligned*/

int alloc_ptr_aligned() {

	char array[DECENT_HEAP_SIZE]; 
	void *block;
	bool aligned_f = false;

	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);
	
	block = hl_alloc(&array, 11);
	

	// you may find this helpful. feel free to remove
    #ifdef PRINT_DEBUG
    	printf("blockptr = %16lx\n", (unsigned long)block);
    	printf("mask =     %16lx\n", (unsigned long)(ALIGNMENT -1));
    	printf("---------------------------\n");
    	printf("ANDED =    %16lx\n", (unsigned long)block & (ALIGNMENT - 1));
    	printf("!ANDED (ALIGNED) =   %6d\n", !((unsigned long)block & (ALIGNMENT - 1)));
    #endif

	aligned_f = !((unsigned long)block & (ALIGNMENT - 1));

	printf("%d) hl_alloc return ptr has correct alignment?", num_tests); 
	if (aligned_f) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
	
}

int main() {
	
	int num_passes = 0;
	
	printf("Beginning Correctness Test!\n---------------------------\n");
	
	num_passes += resize_copies_values();
    num_passes += alloc_ptr_aligned();
	num_passes += resize_blockpr_is_0();
	num_passes += init_failed_heapsize();
	num_passes += alloc_failed_block_size();
	num_passes += resize_failed_newBlockSize();
	num_passes += resize_failed_blockptr();
	num_passes += resize_ptr_aligned();
	
	
	printf("Num_Passed_Tests: %d\n", num_passes);
	printf("Num_Tests: %d\n", num_tests);
	printf("Pass_Rate: %.02f\n", num_passes/(double)num_tests);
	
	return 0;
}
