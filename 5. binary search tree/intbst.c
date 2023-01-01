#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);




////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	

	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0);

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
			
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}






////////////////////////////////////////////////////////////////////////////////

TREE *BST_Create( void){

	TREE *mytree = (TREE*)malloc(sizeof(TREE));
	mytree->root = NULL;
	return mytree;

}

void BST_Destroy( TREE *pTree){

	if(pTree->root == NULL)
		free(pTree);
	else
		_destroy(pTree->root);

	

		
}
static void _destroy( NODE *root){

}





int BST_Insert( TREE *pTree, int data)
{

	NODE *newnode = _makeNode(data);
	
	if(!newnode)
		return 0;


	if(pTree->root == NULL) //first node insert 첫번째 노드가 root노드가 되는거 맞지?
	{
		pTree->root = newnode;
		return 1;
	}


	_insert(pTree->root, newnode);

	return 1;
}
static void _insert( NODE *root, NODE *newPtr)
{
	if(root != NULL)
	{


		if(root->data > newPtr->data)
		{
			if(root->left == NULL)
				root->left = newPtr;
			else _insert(root->left, newPtr);
		}
		else
		{
			if(root->right == NULL)
				root->right = newPtr;
			else _insert(root->right, newPtr);
		}
	}



}

NODE *_makeNode( int data)
{
	NODE *newnode = (NODE*)malloc(sizeof(NODE));
	newnode->data = data;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;
}


int BST_Delete( TREE *pTree, int dltKey){

	NODE *delnode;
	
	int isroot=0;
	if(pTree->root->data == dltKey)
		isroot=1;

	int success = 0;
	delnode = _delete(pTree->root, dltKey, &success);


	if(isroot==1)
	//삭제한 노드가 pTree의 root일 경우, 새로 root를 정해준다.
		pTree->root = delnode;

	return success;

}

static NODE *_delete( NODE *root, int dltKey, int *success){
//success를 return값으로 삼게 되면, **root 이중 포인터를 사용해야 한다.


 	if (root == NULL)
        return root;


    if (dltKey < root->data)
        root->left = _delete(root->left, dltKey, success);
    //delKey가 루트값보다 작은 경우, 루트의 왼쪽 서브트이에서 삭제를 진행한다. (찾는 과정 다시)

    else if (dltKey > root->data)
        root->right = _delete(root->right, dltKey, success);
    else {
    //키값을 찾은 경우

    	*success = 1;

        if (root->left == NULL) {
            NODE* temp = root->right;
            free(root);
            return temp;
            //temp가 새로운 root가 된어서 함수 재귀적으로.. return
            
        }
        else if (root->right == NULL) {
            NODE* temp = root->left;
            free(root);
            return temp;

        }//왼쪽 서브트리는 있으나 오른쪽 서브트리는 있는 경우
        else{//왼쪽 서브트리와 오른쪽 서브트리가 모두 있는 경우 !!

        	//루트의 값을 루트보다 작은 값들 중 최댓값으로 바꾼다.
        	//오른쪽 서브트리에서 최솟값을 찾는다.
        	NODE *p = root->right;
        	while(p && p->left)
        		p = p->left;

        	root->data = p->data;
        	root->right = _delete(root->right, p->data, success);
        	return root;
        	//오른쪽 서브트리에서 해당 최솟값을 삭제한다.
        }

    }
    return root;
    //리턴값 -> 삭제한 노드

}

int *BST_Retrieve( TREE *pTree, int key)
{
	if(pTree->root == NULL)
		return NULL;

	else
		return &(_retrieve(pTree->root, key)->data);
}

static NODE *_retrieve( NODE *root, int key)
{			
	if(!root)
		return NULL;

	if(root->data > key)
		_retrieve(root->left, key);
	else if(root->data < key)
		_retrieve(root->right, key);
	else//root=key
		return root;

	return NULL;

}

//prints tree using inorder traversal
void BST_Traverse( TREE *pTree){
	
	if(pTree->root)
		_traverse(pTree->root);
	
}
static void _traverse( NODE *root){

	if(root != NULL)
	{
		if(root->left)
			_traverse(root->left);
		printf("%d ", root->data);
		if(root->right)
			_traverse(root->right);

	}

}


/* Print tree using inorder "right-to-left" traversal
*/
void printTree( TREE *pTree)
{
	_inorder_print(pTree->root, 0);
}

static void _inorder_print( NODE *root, int level)
{
	int i;


	if(root != NULL)
	{
		
		
		if(root->right)
			_inorder_print(root->right, level+1);

		for(i=0; i<level; i++)
			printf("\t");
		printf("%d\n", root->data);

		if(root->left)
			_inorder_print(root->left, level+1);

	}

}

//return 1 if the tree is empty; 0 if not
int BST_Empty( TREE *pTree){
	if(pTree->root == NULL)
		return 1;
	else return 0;
}