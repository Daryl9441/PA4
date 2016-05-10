#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>

/* Useful shorhand: casts a pointer to a (char *) before adding */
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

#define SUCCESS 1
#define FAIL 0
#define ALIGNMENT 8
/*Checks the given argument is 8 byte aligned. Returns 1 if true, else 0*/
#define NOT_ALIGN(heapptr)(((unsigned long)heapptr & (ALIGNMENT - 1)) != 0 )
/* Realligns the argument given if it is not 8 byte aligned*/
#define RE_ALIGN(heapptr)(heapptr + (ALIGNMENT - ((unsigned long)heapptr % ALIGNMENT)))

/* You must implement these functions according to the specification given in
 * heaplib.h. You can define any types you like to this file.
 */

typedef struct heap_header_t {
	unsigned int size;
	int bytes_free;
	char *next_free;
} heap_header_t ;

/* This is an example of a helper print function that you may find
 * useful for programming and debugging purposes.  Reminder to always
 * wrap all print statments inside an #ifdef so that the default
 * compile does not include them.  Functton calls and printfs will
 * kill your performance and create huge output files, for which we
 * will have ZERO TOLERANCE.
 */
void print_heap(heap_header_t *heap) {
	printf("heap starts at addr %p\n"
		   "total size = %d\n"
		   "bytes free = %d\n"
		   "next free byte begins at addr %p\n",
		   heap, heap->size, heap->bytes_free, heap->next_free);
}


int hl_init(void *heapptr, unsigned int heap_size) {
	if (heap_size < sizeof(heap_header_t) )
	{
		return FAIL;
	}
	if (NOT_ALIGN(heapptr))
	{
		heapptr = RE_ALIGN(heapptr);
		
	}

	heap_header_t *heap = (heap_header_t *)heapptr;
	if (NOT_ALIGN(heap_size))
	{
		heap_size = RE_ALIGN(heap_size);
	}

	heap->size = heap_size;
	char num_bytes = sizeof(heap_header_t);
	if (NOT_ALIGN(num_bytes))
	{
		num_bytes = RE_ALIGN(num_bytes);
	}
	heap->next_free = ADD_BYTES(heap, num_bytes);
	heap->bytes_free = heap_size - num_bytes;
	
    #ifdef PRINT_DEBUG
        print_heap(heap);
    #endif

   	
	return SUCCESS; // Success!

}

void *hl_alloc(void *heapptr, unsigned int block_size) {

	heap_header_t *heap = (heap_header_t *)heapptr;
	if (block_size > heap->bytes_free )
	{
		return FAIL;
	}
	void *blockptr = heap->next_free;

	//check if block_size is 8 byte aligned
	if (NOT_ALIGN(block_size))
	{
		block_size = RE_ALIGN(block_size);
	}
	
	heap->next_free = heap->next_free + block_size;
	heap->bytes_free = heap->bytes_free - block_size;
	
	return blockptr; // Success!
	
}

void hl_release(void *heapptr, void *blockptr) {

}

void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {


	if (NOT_ALIGN(blockptr))
	{
		return FAIL;
	}
	heap_header_t *heap = (heap_header_t *)heapptr;
	// if size is not changing the memory size.
	
	if (NOT_ALIGN(new_block_size))
	{
		new_block_size = RE_ALIGN(new_block_size);
	}
	//memory not enough
	if (new_block_size > heap->bytes_free )
	{
		return FAIL;
	}

    char *newHeap = (char *)hl_alloc(heapptr, new_block_size);
    //if blockptr is 0, then resize should just allocate space like alloc
    //otherwise it should copy data from the old address, blockptr
    if (blockptr != 0)
	{
		memcpy(newHeap, blockptr, new_block_size);
	}
	
	return newHeap;

	//return hl_alloc(heapptr, new_block_size);

}

