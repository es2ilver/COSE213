#define SHOW_STEP 0 // 제출시 0
#define BALANCING 1 // 제출시 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( char *data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}






// 코드 작성 ////////////////////////////////////////////////////////////////////


AVL_TREE *AVL_Create( void)
{
	AVL_TREE* AVL = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	AVL->root = NULL;
	AVL->count = 0;

	return AVL;
}


/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree)
{
	if(pTree) _destroy(pTree->root);
	free(pTree);
}

void _destroy( NODE *root)
{
	if(!root) return; //초기값
	else{
		_destroy(root -> left);
		_destroy(root -> right);
		free(root->data);
		free(root);
	}
}


/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data){

	NODE* newnode = _makeNode(data);
	pTree->root = _insert(pTree->root, newnode);
	
	pTree->count ++;
	
	return 1;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
NODE *_insert( NODE *root, NODE *newPtr){

	if(!root){
		root = newPtr;
		return root;
	}

	if(strcmp(root->data, newPtr->data) > 0){
		root->left = _insert(root->left, newPtr);


		if(BALANCING){

			if(getHeight(root->left) > getHeight(root->right)+1){ //LEFT가 더 큰 경우 -> LL, LR

				if(getHeight(root->left->left)>getHeight(root->left->right)){ //LL
					root = rotateRight(root);
				}
				else{//LR

					root->left = rotateLeft(root->left);
					root = rotateRight(root);

				}
			}

		}
	}
	else{
		root->right = _insert(root->right, newPtr);


		if(BALANCING)
		{
			if(getHeight(root->right) > getHeight(root->left)+1){ //right가 더 큰 경우 -> LL, LR

				if(getHeight(root->right->right)>getHeight(root->right->left)){ //RR
					root = rotateLeft(root);
				}
				else{//RL

					root->right = rotateRight(root->right);
					root = rotateRight(root);
				}
			}
		}



	}

	root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
	
	return root;
}

NODE *_makeNode( char *data)
{
	NODE* newnode = (NODE*)malloc(sizeof(NODE));
	newnode->data = strdup(data);
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->height = 0; //Insert 할때 높이 추가 하나 해주니까 여기선 0으로 둬야 함

	return newnode;
}


char *AVL_Retrieve( AVL_TREE *pTree, char *key)
{
	if (!_retrieve(pTree->root, key)) return 0;
	return _retrieve(pTree->root, key)->data;
}


NODE *_retrieve( NODE *root, char *key)
{
	if(!root) return NULL;
	else {
		if (strcmp(key, root->data) < 0) {
			return _retrieve(root->left, key);
		}
		else if (strcmp(key, root->data) > 0) {
			return _retrieve(root->right, key);
		}
		else return root;
	}
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree){
	if(pTree->root) _traverse(pTree->root);
}

void _traverse( NODE *root){
	
	if(root != NULL)
	{
		if(root->left)
			_traverse(root->left);
		printf("%s ", root->data);
		if(root->right)
			_traverse(root->right);

	}

}


void printTree( AVL_TREE *pTree){
	_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
void _infix_print( NODE *root, int level){

	int i;


	if(!root) return;	
		
	if(root->right) _infix_print(root->right, level+1);

	for(i=0; i<level; i++)
		printf("\t");
	printf("%s\n", root->data);

	if(root->left) _infix_print(root->left, level+1);

	
}


int getHeight( NODE *root){
	if(!root) return 0;
	else return root->height;
}


NODE *rotateRight( NODE *root){ //"OUT of balance인 부분의 ROOT"
	

	NODE* p;
	
	p = root->left;
	if(!p) return root;

	root->left = p->right;
	p->right = root;

	root->height = max(getHeight(root->left), getHeight(root->right))+1;
	p->height = max(getHeight(p->left), getHeight(p->right))+1;



	return p;
}

NODE *rotateLeft( NODE *root){


	NODE *p;
	
	p = root->right;
	if(!p) return root;

	root->right = p->left;
	p->left = root;

	root->height = max(getHeight(root->left), getHeight(root->right))+1;
	p->height = max(getHeight(p->left), getHeight(p->right))+1;

	return p;
}


//static함수 다시 쓸땐 static 떼야 함
