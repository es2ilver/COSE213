#include <stdlib.h> // malloc


#include "adt_dlist.h"



static int _insert( LIST *pList, NODE *pPre, void *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu);



LIST *createList( int (*compare)(const void *, const void *)){


	LIST *mylist = (LIST*)malloc(sizeof(LIST));

	mylist->count = 0;
	mylist->head = NULL;
	mylist->rear = NULL;
	mylist->compare = compare;

	return mylist;
}


void destroyList( LIST *pList, void (*callback)(void *)){


	NODE* current = pList->head;
	NODE* next;


	while(current!=NULL){


		next = current->rlink;
		free(current->dataPtr);
		free(current);
		current = next; //꺄아악


	}
	pList->head = NULL;

}

int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *)){


	NODE* newnode = (NODE *)malloc(sizeof(NODE));

	newnode->llink = NULL;
	newnode->rlink = NULL;
	newnode->dataPtr = dataInPtr;
	

	if(pList->count == 0){

		pList->head = newnode;
		pList->rear = newnode;

		pList->count++;
		
		return 1;

	} //첫노드


	void *dataOutPtr;
	NODE *pPre;
	NODE *pLoc;

	
	if(searchList(pList, dataInPtr, &dataOutPtr)){ //리스트에 있는 경우

		callback(dataOutPtr, dataInPtr);
		free(newnode);
		return 2;
	}

	else{ //리스트에 없는 경우

		//pPre = 선행자, 삽입될 노드의 앞
		if(!_search( pList, &pPre, &pLoc, dataInPtr)){

			


			//CASE1. 맨 앞에 위치하는 경우
			if(pPre==NULL){

				

				newnode->llink = NULL;
				newnode->rlink = pList->head;

				pList->head->llink = newnode;
				pList->head = newnode;

				
			}

			//CASE2. 중간에 위치하는 경우
			else if(pPre!=NULL && pPre->rlink!=NULL){	

				


				newnode->llink = pPre;
				newnode->rlink = pPre->rlink;


				newnode->rlink->llink = newnode;
				pPre->rlink = newnode;

	
			}

			else{//CASE3. 맨 끝에 위치하는 경우 (=리스트에서 끝까지 못찾은 경우)

				

				newnode->llink = pPre;
				newnode->rlink = NULL;

				
				pList->rear->rlink = newnode;
				pList->rear = newnode;

				
			}
			


		}

		

		pList->count++;
		return 1;
	}
}



int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr){

	if(!searchList(pList, keyPtr, dataOutPtr)){
		return 0;
	}
	else{


		NODE *p;
		p = pList->head;
		NODE *delete;


		while(1){

			
			
			if(!pList->compare(p->dataPtr, keyPtr)){

				delete = p;
				*dataOutPtr = p->dataPtr;
				break;
			}
			p = p->rlink;


		}

		
		//CASE1 맨처음인 경우
		//CASE2 중간인 경우
		//CASE3 맨 마지막인 경우

		if(!delete->llink){

			pList->head = delete->rlink;
			delete->rlink->llink = NULL;
			
		}

		else if(delete->llink!=NULL && delete->rlink!=NULL){

			delete->llink->rlink = delete->rlink;
			delete->rlink->llink = delete->llink;
		 	

		}
		else{

			pList->rear = delete->llink;
			delete->llink->rlink = NULL;

		}


		
		free(delete);
		delete = NULL;

		pList->count -- ;


		return 1;

	}
}



int searchList( LIST *pList, void *pArgu, void **dataOutPtr){
	NODE *p = pList->head;


	while(p!= NULL){

		
		
		if(!pList->compare(p->dataPtr, pArgu)){
			(*dataOutPtr) = p->dataPtr;
			return 1;
		}
		

		p = p->rlink;

	}

	return 0;

}


int countList( LIST *pList){
	return pList->count;
}

int emptyList( LIST *pList){
	if(pList->count==0){
		return 0;
	}
	else
		return 1;
}




/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const void *)){
	NODE *p = pList->head;

	while(p!= NULL){

		callback(p->dataPtr);
		p = p->rlink;
		
	}
}

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const void *)){
	NODE *p = pList->rear;

	while(p!= NULL){

		callback(p->dataPtr);
		p = p->llink;
		
	}
}



static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu){

	
	NODE *p = pList->head;//헤드에서 탐색 시작
	

	*pPre = NULL;

	while(1){


		int ret = pList->compare(p->dataPtr, pArgu);

		if(!ret){
			
			*pLoc = p;
			*pPre = p->llink;
			return 1;
		}
		
		//키값을 못찾음, 중간에 삽입해야 하는 경우
		if( ret > 0){

			*pPre = p->llink;
			return 0;
		}

		if(!(p->rlink)){ //끝까지 탐색, 못찾음

			*pPre = p;
			return 0;


		}
		else
			p = p->rlink;//뒤쪽으로 탐색

	}

}