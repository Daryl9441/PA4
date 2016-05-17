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
	/*this is the atart of the node pointer*/
	node *newNode = (node *)next_free;
	newNode->size = heap->bytes_free;
	MARK_NOT_USED (newNode->size);
	newNode->next = newNode;
	newNode->prev = NULL;
	// heap->head = newNode;
	// heap->tail = NULL;
	// //heap->head->size = heap->bytes_free;

    return SUCCESS;
}

	
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
    	remove_from_list(first_Node, fit);
    	void *blockptr = make_node_free((char *)fit , fit->size);
    	heap->bytes_free = SUB(heap->bytes_free, block_size);
    	return blockptr;
    }

	//void *blockptr = heap->next_free;
	// if (NOT_ALIGN(block_size))
	// {
	// 	block_size = RE_ALIGN(block_size);
	
	// heap->next_free = heap->next_free + block_size;
	// heap->bytes_free = heap->bytes_free - block_size;
	
	// return blockptr; // Success
	return FAIL;



}

void hl_release(void *heapptr, void *blockptr) {
	if (blockptr != 0 && !(NOT_ALIGN(blockptr)))
	{

		header *heap = (header *) heapptr;
		node *blockToRelease = (node *) blockptr;
		node *first_Node = heap->start_Node;
		MARK_NOT_USED (heap->size);
		make_node_free((char *)blockToRelease, blockToRelease->size);
		insert(first_Node , blockToRelease);
		heap->bytes_free = heap->bytes_free + blockToRelease->size;
		// blockToRelease->next = heap;
		// blockToRelease->prev = heap->prev;
		// heap->prev = blockToRelease;
		// bytes_free = bytes_free +  blockToRelease->size;

		
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

    //printf("%u\n bytes_free =  %u new_block_size =  ", bytes_free, new_block_size);
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
        //insert(first_Node, fit);
        insert(first_Node, new_node);
    }
    
    return fit;
}

static void remove_from_list(node *first_Node, node *fit)
{
	if (first_Node == fit)
    {
        if (first_Node == fit->next ) //its the only node in the linkelist
        {
            first_Node->prev = NULL;
            first_Node->next = NULL;
        }
        else{ // its at the beginning of the linkedlist but with subsequent nodes
        	first_Node->prev = NULL;
        	first_Node->next->prev = NULL;
        }
    }
    else
    {
    	fit->next->prev = fit->prev;
    	fit->prev->next = fit->next;


    }
    fit->prev = NULL;
    fit->next = NULL;
}
    //return fit;
//     node *first = first_Node;
//     if (first_Node == fit)
//     {
//         if (first_Node == fit->next) 
//         {
//             first_Node->prev = NULL;
//         }
//     }
//     else
//      {
//         while (first->next != fit) 
//         {
//             first = first->next;
//         }
//         if (first->next == fit->next) {
//             first->next->prev = first;
//         }
//     }
// }
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

static void insert(node *first_Node, node *x)
{
    if (first_Node == NULL || x < first_Node) 
    {
        x->prev = NULL;
        x->next = first_Node;
        first_Node = x;
    } 
    else
    {
    	x->prev = first_Node;
    	x->next = first_Node->next;
    	if (first_Node->next != NULL)
    	{
    		first_Node->next->prev = x;
    	}
    	first_Node->next = x;

    }



    // else {
    //     while (front->next != NULL && front->next <= x) {
    //         front = front->next;
    //     }
    //     if (front->next == x) {
    //         return;
    //     }
    //     x->prev = front;
    //     x->next = front->next;
    // }
    // if (x->prev) {
    //     x->prev->next = x;
    // }
    // if (x->next) {
    //     x->next->prev = x;
    // }
}

