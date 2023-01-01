#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index){	
	
	if(index>1)
	{	
		void * parents = heap->heapArr[index/2];
		if(heap->compare(parents, heap->heapArr[index]) < 0)
		{	
			heap->heapArr[index/2] = heap->heapArr[index];
			heap->heapArr[index] = parents;
			_reheapUp(heap, index/2);


		}
	}
}
static void _reheapDown( HEAP *heap, int index){

	if(index <= heap->last){
		void * parents = heap->heapArr[index];
		if(index*2 <= heap->last){//왼쪽 있음
			if(index*2+1 <= heap->last)
			{//오른쪽도 있음




				if(heap->compare(heap->heapArr[index*2], heap->heapArr[index*2+1]) > 0)
				{	
					if(heap->compare(heap->heapArr[index], heap->heapArr[index*2]) < 0 )
					{
						heap->heapArr[index] = heap->heapArr[index*2];
						heap->heapArr[index*2] = parents;
						_reheapDown(heap, index*2);

					}
					
				}
				else
				{
				
					if(heap->compare(heap->heapArr[index], heap->heapArr[index*2+1]) < 0 )
					{
						heap->heapArr[index] = heap->heapArr[index*2+1];
						heap->heapArr[index*2+1] = parents;
						_reheapDown(heap, index*2+1);

					}
				}


			}
			else
			{
				if(heap->compare(heap->heapArr[index], heap->heapArr[index*2]) < 0 )
					{
						heap->heapArr[index] = heap->heapArr[index*2];
						heap->heapArr[index*2] = parents;
						_reheapDown(heap, index*2);

					}
			}
		}

	}

}


HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2))
{
	HEAP *heap = (HEAP *)malloc(sizeof(HEAP));

	heap->last = 0 ;
	heap->capacity = capacity;
	heap->compare = compare;

	heap->heapArr = (void*)malloc(sizeof(void*)*capacity +1);

	return heap;
}

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap)
{
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr){
	
	if(heap->last+1 == heap->capacity)
	{
		heap->capacity*=2;
		heap->heapArr = (void*)realloc(heap->heapArr, sizeof(void*)*heap->capacity +1);
	}
	heap->last++;
	heap->heapArr[heap->last] = dataPtr;

	_reheapUp(heap, heap->last);


	return 1;
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr){


	if(heap_Empty(heap))
		return 0;

	*dataOutPtr = heap->heapArr[1];
	heap->heapArr[1] = heap->heapArr[heap->last];
	
	

	_reheapDown(heap, 1);
	heap->last--;
	return 1;
}
/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap){
	return (heap->last == 0);
}

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data)){
	int i=0;
	for(i=1; i<=heap->last; i++)
	{
		print_func(heap->heapArr[i]);
	}
	printf("\n");
}