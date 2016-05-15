#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>

/* Useful shorhand: casts a pointer to a (char *) before adding */
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

/*making it to 8 byte aligned*/
#define ALIGNMENT 8

/*check if bas address if 8 byte aligned*/
//#define NOT_ALIGN(heapptr)(((char)heapptr % ALIGNMENT) != 0)
#define ALIGN(a)((unsigned long)a & (ALIGNMENT - 1))
/* Realligns the argument given if it is not 8 byte aligned*/
#define RE_ALIGN(a)(a + (ALIGNMENT - ((unsigned long)a % ALIGNMENT)))

#define SUCCESS 1
#define FAIL 0

// typedef available{
// 	int bytes_free;
// }available;

typedef header{
	unsigned in size;
	header *prev;
	header *next;
	//available *freeb;
}header;

/*
 * Include a description of your approach here.
 *
 */

int hl_init(void *heapptr, unsigned int heap_size) {

	if (heap_size < sizeof(header) )
	{
		return FAIL;
	}
	//Checking if heapptr is 8 byte aligned. Realigned if not
	if (NOT_ALIGN(heapptr))
	{
		heapptr = RE_ALIGN(heapptr);
		
	}

	header *heap = (header *)heapptr;
	//Checking if heap_size is 8 byte aligned and realigning if it is not
	if (NOT_ALIGN(heap_size))
	{
		heap_size = RE_ALIGN(heap_size);
	}

	heap->size = heap_size;
	char num_bytes = sizeof(header) + sizeof(available);// this is the size of the header
	//Checking if num_bytes is 8 byte aligned and realigning if it is not
	if (NOT_ALIGN(num_bytes))
	{
		num_bytes = RE_ALIGN(num_bytes);
	}
	//heap->next_free = ADD_BYTES(heap, num_bytes);
	//heap->bytes_free = heap_size - num_bytes;
	unsigned int  avail = heap_size - num_bytes;//space available
	header nextheap = (header) ADD_BYTES(heap, num_bytes);//new heap
	nextheap->size = avail;
	heap->prev =nextheap;
	heap->next =nextheap;

    return SUCCESS;

}

void *hl_alloc(void *heapptr, unsigned int block_size) {
	heap_header_t *heap = (header *)heapptr;
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
	header fit = find_fit(heap,block_size);
	if ( fit== NULL)
	{
		
	}
	else{
		if (fit->size > block_size)
		{
			unsigned int toADD = fit->size - block_size;
			header newH = (heap)ADD_BYTES(fit, toADD);
			newH->size = toADD;
			newH->next = fit->next;
			newH->prev = fit->prev;
			fit->next->prev = newH;
			fit->prev->next = newH;
			fit->size = block_size;
			return (void *) fit;

		}
		fit->next->prev = fit->prev;
		fit->prev->next = fit->next;
		return (void *) fit

	}
	// //heap->next_free = heap->next_free + block_size;
	// heap->bytes_free = heap->bytes_free - block_size;
	
	// return blockptr; // Success!

}

void hl_release(void *heapptr, void *blockptr) {

}

void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {

  return NULL; // Failed

}

header find_fit(header heap, heap_size){
	while(heap !=NULL){
		if (heap->size >= heap_size){
			return heap;
		}
		else {
			heap = heap->next;
		}
		
	}

}