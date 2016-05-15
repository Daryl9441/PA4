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
#define SUB(a,b)((char)a - (char)b)

/* You must implement these functions according to the specification given in
 * heaplib.h. You can define any types you like to this file.
 */

static  int bytes_free = 0;
typedef struct header{
	unsigned int size;
	struct header *prev;
    struct header *next;
	//int bytes_free;
}header;
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
	char num_bytes = sizeof(header);// + sizeof(available);// this is the size of the header
	//Checking if num_bytes is 8 byte aligned and realigning if it is not
	if (NOT_ALIGN(num_bytes))
	{
		num_bytes = RE_ALIGN(num_bytes);
	}
	//heap->next_free = ADD_BYTES(heap, num_bytes);
	//heap->bytes_free = heap_size - num_bytes;
	unsigned int  avail = heap_size - num_bytes;//space available
	void *ptr = ADD_BYTES(heap, num_bytes);
 	header *nextheap = (header *) ptr;//new heap
	//header *nextheap = (header) ADD_BYTES(heap, num_bytes);//new heap
	nextheap->size = avail;
	heap->prev =nextheap;
	heap->next =nextheap;
	bytes_free = SUB(heap_size,num_bytes);

    return SUCCESS;



}

header *find_fit(header *heap, unsigned int heap_size){
	while(heap !=NULL){
		if (heap->size >= heap_size){
			return heap;
		}
		else {
			heap = heap->next;
		}
		
	}
	return heap;
}

void *hl_alloc(void *heapptr, unsigned int block_size) {
		header *heap = (header *)heapptr;
	if (block_size > bytes_free )
	{
		return FAIL;
	}
	//void *blockptr = heap->next_free;

	//check if block_size is 8 byte aligned
	if (NOT_ALIGN(block_size))
	{
		block_size = RE_ALIGN(block_size);
	}
	header *fit = find_fit(heap,block_size);

	if ( fit == NULL)
	{
		
	}
	//heap->bytes_free = heap->bytes_free - block_size;
	else{
		bytes_free = SUB(bytes_free , block_size);
		if (fit->size > block_size)
		{
			unsigned int toADD = fit->size - block_size;
			void *added = ADD_BYTES(fit, toADD);
			header *newH = (header *)added;
			newH->size = toADD;
			newH->next = fit->next;
			newH->prev = fit->prev;
			fit->next->prev = newH;
			fit->prev->next = newH;
			fit->size = block_size;
			//heap->bytes_free = heap->bytes_free - toADD;
			return (void *) fit;

		}
		else{
			fit->next->prev = fit->prev;
		    fit->prev->next = fit->next;
		    //heap->bytes_free = heap->bytes_free - block_size;
		    return (void *) fit;
		}

	}
	return 0;
	// //heap->next_free = heap->next_free + block_size;
	//heap->bytes_free = heap->bytes_free - block_size;
	
	// return blockptr; // Success!


  // return NULL; // Failed

}

void hl_release(void *heapptr, void *blockptr) {
	// if (NOT_ALIGN(blockptr))
	// {
	// 	return NULL;
	// }
	if (blockptr != 0 && !(NOT_ALIGN(blockptr)))
	{
		header *heap = (header *) heapptr;
		header *blockToRelease = (header *) heapptr;
		blockToRelease->next = heap;
		blockToRelease->prev = heap->prev;
		heap->prev = blockToRelease;
		bytes_free = bytes_free +  blockToRelease->size;

		
	}
	


}

void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {

  	if (NOT_ALIGN(blockptr))
	{
		return FAIL;
	}
	//header *heap = (header *)heapptr;
	//check if new_block_size is 8 byte aligned
	
	if (NOT_ALIGN(new_block_size))
	{
		new_block_size = RE_ALIGN(new_block_size);
	}
	//memory not enough
	if (new_block_size > bytes_free )
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






// #include <stdlib.h>
// #include <stdio.h>
// #include "heaplib.h"
// #include <string.h>

// /* Useful shorhand: casts a pointer to a (char *) before adding */
// #define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

// #define SUCCESS 1
// #define FAIL 0
// #define ALIGNMENT 8
// /*Checks the given argument is 8 byte aligned. Returns 1 if true, else 0*/
// #define NOT_ALIGN(heapptr)(((unsigned long)heapptr & (ALIGNMENT - 1)) != 0 )
// /* Realligns the argument given if it is not 8 byte aligned*/
// #define RE_ALIGN(heapptr)(heapptr + (ALIGNMENT - ((unsigned long)heapptr % ALIGNMENT)))

// /* You must implement these functions according to the specification given in
//  * heaplib.h. You can define any types you like to this file.
//  */
// typedef struct header{
// 	unsigned int size;
// 	struct header *prev;
//     struct header *next;
// 	int bytes_free;
// 	//available *freeb;
// }header;

// header find_fit(header heap, unsigned int heap_size);

// /*
//  * Include a description of your approach here.
//  *
//  */

// int hl_init(void *heapptr, unsigned int heap_size) {

// 	if (heap_size < sizeof(header) )
// 	{
// 		return FAIL;
// 	}
// 	//Checking if heapptr is 8 byte aligned. Realigned if not
// 	if (NOT_ALIGN(heapptr))
// 	{
// 		heapptr = RE_ALIGN(heapptr);
		
// 	}

// 	header *heap = (header *)heapptr;
// 	//Checking if heap_size is 8 byte aligned and realigning if it is not
// 	if (NOT_ALIGN(heap_size))
// 	{
// 		heap_size = RE_ALIGN(heap_size);
// 	}

// 	//heap->size = heap_size;
// 	char num_bytes = sizeof(header) ;//+ sizeof(available);// this is the size of the header
// 	//Checking if num_bytes is 8 byte aligned and realigning if it is not
// 	if (NOT_ALIGN(num_bytes))
// 	{
// 		num_bytes = RE_ALIGN(num_bytes);
// 	}
// 	//heap->next_free = ADD_BYTES(heap, num_bytes);
// 	heap->bytes_free = heap_size - num_bytes;
// 	unsigned int  avail = heap_size - num_bytes;//space available
// 	void *ptr = ADD_BYTES(heap, num_bytes);
// 	header *nextheap = (header *) ptr;//new heap
// 	nextheap->size = avail;
// 	heap->prev =nextheap;
// 	heap->next =nextheap;

//     return SUCCESS;

// }

// void *hl_alloc(void *heapptr, unsigned int block_size) {
// 	header *heap = (header *)heapptr;
// 	if (block_size > heap->bytes_free )
// 	{
// 		return FAIL;
// 	}
// 	//void *blockptr = heap->next_free;

// 	//check if block_size is 8 byte aligned
// 	if (NOT_ALIGN(block_size))
// 	{
// 		block_size = RE_ALIGN(block_size);
// 	}
// 	header *fit = find_fit(heap,block_size);
// 	if ( fit == NULL)
// 	{
		
// 	}
// 	else{
// 		if (fit->size > block_size)
// 		{
// 			unsigned int toADD = fit->size - block_size;
// 			header *newH = (heap)ADD_BYTES(fit, toADD);
// 			newH->size = toADD;
// 			newH->next = fit->next;
// 			newH->prev = fit->prev;
// 			fit->next->prev = newH;
// 			fit->prev->next = newH;
// 			fit->size = block_size;
// 			return (void *) fit;

// 		}
// 		fit->next->prev = fit->prev;
// 		fit->prev->next = fit->next;
// 		return (void *) fit;

// 	}
// 	// //heap->next_free = heap->next_free + block_size;
// 	// heap->bytes_free = heap->bytes_free - block_size;
	
// 	// return blockptr; // Success!

// }

// void hl_release(void *heapptr, void *blockptr) {

// }

// void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {

//   return NULL; // Failed

// }

// header find_fit(header *heap, unsigned int heap_size){
// 	while(heap !=NULL){
// 		if (heap->size >= heap_size){
// 			return heap;
// 		}
// 		else {
// 			heap = heap->next;
// 		}
		
// 	}
// 	return heap;

// }
// #include <stdlib.h>
// #include <stdio.h>
// #include "heaplib.h"
// #include <string.h>

// /* Useful shorhand: casts a pointer to a (char *) before adding */
// #define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

// #define SUCCESS 1
// #define FAIL 0
// #define ALIGNMENT 8
// /*Checks the given argument is 8 byte aligned. Returns 1 if true, else 0*/
// #define NOT_ALIGN(heapptr)(((unsigned long)heapptr & (ALIGNMENT - 1)) != 0 )
// /* Realligns the argument given if it is not 8 byte aligned*/
// #define RE_ALIGN(heapptr)(heapptr + (ALIGNMENT - ((unsigned long)heapptr % ALIGNMENT)))

// /* You must implement these functions according to the specification given in
//  * heaplib.h. You can define any types you like to this file.
//  */

// typedef struct heap_header_t {
// 	unsigned int size;
// 	int bytes_free;
// 	char *next_free;
// } heap_header_t ;

// /* This is an example of a helper print function that you may find
//  * useful for programming and debugging purposes.  Reminder to always
//  * wrap all print statments inside an #ifdef so that the default
//  * compile does not include them.  Functton calls and printfs will
//  * kill your performance and create huge output files, for which we
//  * will have ZERO TOLERANCE.
//  */
// void print_heap(heap_header_t *heap) {
// 	printf("heap starts at addr %p\n"
// 		   "total size = %d\n"
// 		   "bytes free = %d\n"
// 		   "next free byte begins at addr %p\n",
// 		   heap, heap->size, heap->bytes_free, heap->next_free);
// }


// int hl_init(void *heapptr, unsigned int heap_size) {
// 	if (heap_size < sizeof(heap_header_t) )
// 	{
// 		return FAIL;
// 	}
// 	//Checking if heapptr is 8 byte aligned. Realigned if not
// 	if (NOT_ALIGN(heapptr))
// 	{
// 		heapptr = RE_ALIGN(heapptr);
		
// 	}

// 	heap_header_t *heap = (heap_header_t *)heapptr;
// 	//Checking if heap_size is 8 byte aligned and realigning if it is not
// 	if (NOT_ALIGN(heap_size))
// 	{
// 		heap_size = RE_ALIGN(heap_size);
// 	}

// 	heap->size = heap_size;
// 	char num_bytes = sizeof(heap_header_t);// this is the size of the header
// 	//Checking if num_bytes is 8 byte aligned and realigning if it is not
// 	if (NOT_ALIGN(num_bytes))
// 	{
// 		num_bytes = RE_ALIGN(num_bytes);
// 	}
// 	heap->next_free = ADD_BYTES(heap, num_bytes);
// 	heap->bytes_free = heap_size - num_bytes;
	
//     #ifdef PRINT_DEBUG
//         print_heap(heap);
//     #endif

   	
// 	return SUCCESS; // Success!

// }

// void *hl_alloc(void *heapptr, unsigned int block_size) {

// 	heap_header_t *heap = (heap_header_t *)heapptr;
// 	if (block_size > heap->bytes_free )
// 	{
// 		return FAIL;
// 	}
// 	void *blockptr = heap->next_free;

// 	//check if block_size is 8 byte aligned
// 	if (NOT_ALIGN(block_size))
// 	{
// 		block_size = RE_ALIGN(block_size);
// 	}
	
// 	heap->next_free = heap->next_free + block_size;
// 	heap->bytes_free = heap->bytes_free - block_size;
	
// 	return blockptr; // Success!
	
// }

// void hl_release(void *heapptr, void *blockptr) {

// }

// void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {


// 	if (NOT_ALIGN(blockptr))
// 	{
// 		return FAIL;
// 	}
// 	heap_header_t *heap = (heap_header_t *)heapptr;
// 	//check if new_block_size is 8 byte aligned
	
// 	if (NOT_ALIGN(new_block_size))
// 	{
// 		new_block_size = RE_ALIGN(new_block_size);
// 	}
// 	//memory not enough
// 	if (new_block_size > heap->bytes_free )
// 	{
// 		return FAIL;
// 	}

//     char *newHeap = (char *)hl_alloc(heapptr, new_block_size);
//     //if blockptr is 0, then resize should just allocate space like alloc
//     //otherwise it should copy data from the old address, blockptr
//     if (blockptr != 0)
// 	{
// 		memcpy(newHeap, blockptr, new_block_size);
// 	}
	
// 	return newHeap;

// 	//return hl_alloc(heapptr, new_block_size);

// }

