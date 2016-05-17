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
#define SUB(a,b)((unsigned int)a - (unsigned int)b)
#define ALIGNPTR(ptr)((void )(((unsigned long)ptr + ALIGNMENT) & ALIGNMENT))
#define HEADER_SIZE (sizeof(struct header))
#define NODE_SIZE (sizeof(struct node))
#define META_SIZE (NODE_SIZE + HEADER_SIZE)
#define MARK_USED(a)((a) |= 1)
#define MARK_NOT_USED(a)((a) &= (~1))
#define PREV_POINTER(x) ((header)(x)-1)

//#define ALIGN(ptr)((ptr + ALIGNMENT) & ALIGNMENT)

/* You must implement these functions according to the specification given in
 * heaplib.h. You can define any types you like to this file.
 */

typedef struct header
{
    unsigned int size;
    int bytes_free;	
    void *start_Node;
	
} header;
typedef struct node 
{
	unsigned int size;
	struct node *prev; //predicessor
	struct node *next;//sucessor
}node;

// typedef struct header {
// 	unsigned int size;
// 	int bytes_free;
// 	node *head;
// 	node *tail;
// } header ;

static node *find_fit(node *NodePTR, unsigned int block_size);
static node *split_block(node *first_Node, node *fit, unsigned int block_size);
static void remove_from_list(node *first_Node, node *fit);
static node *make_node_free(char *start, unsigned int block_size) ;
static void insert(node *first_Node, node *x);


int hl_init(void *heapptr, unsigned int heap_size) {

	ALIGNPTR (heap_size);
	char num_bytes = META_SIZE;
	ALIGNPTR(num_bytes);
	if (heap_size < sizeof(META_SIZE) )
	{
		return FAIL;
	}
	ALIGNPTR(heapptr);


	//printf("%u\n heap_size before", heap_size);
	//printf("%u\n heap_size after", heap_size);
	header *heap = (header *)heapptr;
	heap->size = heap_size;
	heap->bytes_free = SUB(heap_size ,num_bytes);
	char *next_free = ADD_BYTES(heap, num_bytes);
	heap->start_Node = next_free;
	node *newNode = (node *)next_free;
	newNode->size = heap->bytes_free;
	MARK_NOT_USED (newNode->size);
	newNode->next = newNode;
	newNode->prev = NULL;
	// heap->head = newNode;
	// heap->tail = NULL;
	// //heap->head->size = heap->bytes_free;

    return SUCCESS;

	
	//int META_SIZE = (sizeof(header) + sizeof(node));
void *hl_alloc(void *heapptr, unsigned int block_size) {
	ALIGNPTR(block_size);
	ALIGNPTR(heapptr);
	header *heap = (header *)heapptr;
	if (block_size > heap->bytes_free )
	{
		return FAIL;
	}
	node *first_Node = (node *)heap->start_Node; 
    node *fit = find_fit(first_Node,block_size);
    if (fit !=NULL)
    {
    	fit = split_block(first_Node, fit, block_size);
    }

	//void *blockptr = heap->next_free;
	// if (NOT_ALIGN(block_size))
	// {
	// 	block_size = RE_ALIGN(block_size);
		
	}
	
	// heap->next_free = heap->next_free + block_size;
	// heap->bytes_free = heap->bytes_free - block_size;
	
	// return blockptr; // Success!
	return FAIL;



}

void hl_release(void *heapptr, void *blockptr) {
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
	ALIGNPTR(heapptr);
	//header *heap = (header *)heapptr;
	//check if new_block_size is 8 byte aligned
	ALIGNPTR(new_block_size);
	//memory not enough
	header *heap = (header *)heapptr;

    printf("%u\n bytes_free =  %u new_block_size =  ", bytes_free, new_block_size);
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
}


static node *find_fit(node *NodePTR, unsigned int block_size){
	while(NodePTR != NULL){
		if (NodePTR->size >= block_size){
			return NodePTR;
		}
		else {
			NodePTR = NodePTR->next;
		}
		
	}
	return NodePTR;
}
static node *split_block(node *first_Node, node *fit, unsigned int block_size)
{
    char *start = (char*) fit;
    char *split = (char*) fit + block_size;
    unsigned int  split_size = fit->size - block_size;
    

    if (split_size >= NODE_SIZE) 
    {
        remove_from_list(first_Node, fit);
        fit = make_node_free(start, block_size);
        node *new_node = make_node_free(split, split_size);
        insert(first_Node, fit);
        insert(first_Node, new_node);
    }
    
    return fit;
}

static void remove_from_list(node *first_Node, node *fit)
{
    node *first = first_Node;
    if (first_Node == fit)
    {
        if (first_Node == fit->next) 
        {
            first_Node->prev = NULL;
        }
    }
    else
     {
        while (first->next != fit) 
        {
            first = first->next;
        }
        if (first->next == fit->next) {
            first->next->prev = first;
        }
    }
}
static node *make_node_free(char *start, unsigned int block_size) 
{
    node *freedNode = (node *) start;
    //char *checkend = PREV_POINTER(start + block_size);
    char *end =start + block_size;
    header *endH = (header *)end - 1;
   // header *end = PREV_POINTER(start + block_size);
    freedNode->size = block_size;
    MARK_NOT_USED(freedNode->size);
    endH->size = freedNode->size;
    freedNode->prev = NULL;
    freedNode->next = NULL;
    
    return freedNode;
}
/* Add item to the address-ordered list of free nodes */
static void insert(node *first_Node, node *x)
{
    node *front = first_Node;
    if (first_Node == NULL || x < first_Node) {
        x->prev = NULL;
        x->next = first_Node;
        first_Node = x;
    } else {
        while (front->next != NULL && front->next <= x) {
            front = front->next;
        }
        if (front->next == x) {
            return;
        }
        x->prev = front;
        x->next = front->next;
    }
    if (x->prev) {
        x->prev->next = x;
    }
    if (x->next) {
        x->next->prev = x;
    }
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
// 	unsigned in size;
// 	header *prev;
// 	header *next;
// 	//available *freeb;
// }header;

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

// 	heap->size = heap_size;
// 	char num_bytes = sizeof(header) + sizeof(available);// this is the size of the header
// 	//Checking if num_bytes is 8 byte aligned and realigning if it is not
// 	if (NOT_ALIGN(num_bytes))
// 	{
// 		num_bytes = RE_ALIGN(num_bytes);
// 	}
// 	//heap->next_free = ADD_BYTES(heap, num_bytes);
// 	//heap->bytes_free = heap_size - num_bytes;
// 	unsigned int  avail = heap_size - num_bytes;//space available
// 	header nextheap = (header) ADD_BYTES(heap, num_bytes);//new heap
// 	nextheap->size = avail;
// 	heap->prev =nextheap;
// 	heap->next =nextheap;

//     return SUCCESS;

// }

// void *hl_alloc(void *heapptr, unsigned int block_size) {
// 	heap_header_t *heap = (header *)heapptr;
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
// 	header fit = find_fit(heap,block_size);
// 	if ( fit== NULL)
// 	{
		
// 	}
// 	else{
// 		if (fit->size > block_size)
// 		{
// 			unsigned int toADD = fit->size - block_size;
// 			header newH = (heap)ADD_BYTES(fit, toADD);
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
// 		return (void *) fit

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

// header find_fit(header heap, heap_size){
// 	while(heap !=NULL){
// 		if (heap->size >= heap_size){
// 			return heap;
// 		}
// 		else {
// 			heap = heap->next;
// 		}
		
// 	}

// }

// #include <stdlib.h>
// #include <stdio.h>
// #include "heaplib.h"

// /*
//  * Include a description of your approach here.
//  *
//  */
// /*shorthand for ADDING BYTES*/
// #define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

// #define SUCCESS 1
// #define FAIL 0
// #define ALIGNMENT 8
// /*Checks the given argument is 8 byte aligned. Returns 1 if true, else 0*/
// #define NOT_ALIGN(heapptr)(((unsigned long)heapptr & (ALIGNMENT - 1)) != 0 )
// /* Realligns the argument given if it is not 8 byte aligned*/
// #define RE_ALIGN(heapptr)(heapptr + (ALIGNMENT - ((unsigned long)heapptr % ALIGNMENT)))



// typedef struct heap_header_t {
// 	unsigned int size;
// 	int bytes_free;
// 	char *next_free;
// 	heap_header_t **freed;
// 	unsigned length;
// } heap_header_t ;

// // typedef struct Dlinked {
// // 	unsigned int size;
// // 	heap_header_t *next;
// // 	heap_header_t *prev;
// // } linked ;

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
// 	heap->length = 0;
// 	heap->freed[length] = heap->next_free;
	
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

//   return NULL; // Failed

// }

// void remove(heap_header_t* a, unsigned int index)
// {
//     for (unsigned int i = index; i < a->length - 1; i++)
//         a->freed[i] = a->freed[i + 1];

//     --a->length;
// }

// char find_block(heap_header_t heap, unsigned int block_size){
// 	int count = 0
// 	bool found = false;

//     while (count <= heap->length) {
//     	heap_header_t heap2 = (heap_header_t)heap->freed[count]
//         if (heap2->size >= block_size) {
//         	remove(heap, count);
//         	found = true;
//             return heap->length[count];
//         }
//         ++count;
//     }
//     return heap;
// }





