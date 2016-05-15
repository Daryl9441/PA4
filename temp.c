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
#define ALIGN(heapptr)((unsigned long)heapptr & (ALIGNMENT - 1))

#define SUCCESS 1
#define FAIL 0

typedef node{
	unsigned in size;
	node *prev; //predicessor
	node *next;//sucessor
}node;

typedef struct heap_header_t {
	unsigned int size;
	int bytes_free;
	//char *next_free;
	node *head;
	node *tail;
} heap_header_t ;



/*
 * Include a description of your approach here.
 *
 */

int hl_init(void *heapptr, unsigned int heap_size) {
	if (heap_size < sizeof(heap_header_t) )
	{
		return FAIL;
	}
	//Checking if heapptr is 8 byte aligned. Realigned if not
	if (NOT_ALIGN(heapptr))
	{ 
		heapptr = RE_ALIGN(heapptr);
		
	}

	heap_header_t *heap = (heap_header_t *)heapptr;
	//Checking if heap_size is 8 byte aligned and realigning if it is not
	if (NOT_ALIGN(heap_size))
	{
		heap_size = RE_ALIGN(heap_size);
	}

	heap->size = heap_size;
	char num_bytes = sizeof(heap_header_t);// this is the size of the header
	//Checking if num_bytes is 8 byte aligned and realigning if it is not
	if (NOT_ALIGN(num_bytes))
	{
		num_bytes = RE_ALIGN(num_bytes);
	}
	//heap->next_free = ADD_BYTES(heap, num_bytes);
	heap->bytes_free = heap_size - num_bytes;
	heap->head = (node)ADD_BYTES(heap, num_bytes);
	heap->tail = (node)ADD_BYTES(heap, num_bytes);
	heap->head->size = heap_size; 

	
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
	if (NOT_ALIGN(block_size))
	{
		block_size = RE_ALIGN(block_size);

	}
	linked fit = find_fit(heap, block_size);
    if (/* condition */)
    {
    	/* code */
    }
	
	heap->next_free = heap->next_free + block_size;
	heap->bytes_free = heap->bytes_free - block_size;
	heap->list
	
	return blockptr; // Success!


  return NULL; // Failed

}

void hl_release(void *heapptr, void *blockptr) {

}

void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {

  return NULL; // Failed

}

linked find_fit(node a, unsigned int size) 
{
    while (a->next != NULL) {
        if (heap->size >= size) {
            return heap;
        } else {
            heap = next->next;
        }
    }
    return target;
}
 node append(node *ptr) {
    	//list is empty
    	if(tail==null){
    		head= newPred;
    	}
    	//at least one node
    	else{
    		newPred.pred=tail;
    		tail.succ=newPred;
    	}
		tail=newPred;
		size+=1;
		return newPred;
    }
