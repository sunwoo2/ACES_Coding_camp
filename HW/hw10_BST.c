/*
ID: hrkim
LANG: C
TASK: milk
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct node{
	int data;
	int data1;
	struct node* left;
	struct node* right;
} T_NODE;

typedef struct {
	int count;
	T_NODE* root;
} BST_TREE;

BST_TREE* create_bst();
T_NODE* find_smallest_node(T_NODE* root);
T_NODE* find_largest_node(T_NODE* root);
T_NODE* search_bst(T_NODE* root, int key);
T_NODE* add_bst(T_NODE* root, int data, int data1);
T_NODE* del_bst(T_NODE* root, int data, bool* success);
T_NODE* delete_bst(T_NODE* root, int data, bool* success);
bool BST_insert(BST_TREE* tree, int data, int data1);
bool BST_del(BST_TREE* tree, int data);
void preorder(T_NODE* root);
void inorder(T_NODE* root, int milk[], int unit[], int i);
void postorder(T_NODE* root);


int main(void)
{
	FILE *fin, *fout;

	fin = fopen("milk.in", "r");		
	fout = fopen("milk.out", "w");

	assert((fin != NULL) && (fout != NULL)); // stop program if condition is false

//	fscanf(fin, "%d", &size);



	// data 입력
	int N,M;
	fscanf(fin, "%d %d", &N, &M);

	BST_TREE* b1 = create_bst();
	int temp, temp1;
	for(int i=0; i<M; i++){
		fscanf(fin, "%d %d", &temp, &temp1);
		BST_insert(b1, temp, temp1);
	}

	int milk[M];
	int unit[M];
	int i=0;

	inorder(b1->root, milk, unit, i);
	for(int i=0; i<M; i++)
		printf("%d %d\n", milk[i], unit[i]);





	// 최소비용계산
/*	int check = 0;
	int cost = 0; 
	for(int i=0; i<M; i++){
		for(int j=1; j<=m1[i].n; j++){
			if(check==N)
				break;
			check++;
			cost += m1[i].m;
		}
		if(check==N)
			break;
	}

	fprintf(fout, "%d\n", cost);
*/





//	fprintf(fout, "%d\n", max_num);

	fclose(fin);
	fclose(fout);

	exit(0);
}

BST_TREE*
create_bst(){
	BST_TREE* tree = (BST_TREE*)malloc(sizeof(BST_TREE));
	if(!tree) return false;

	tree->root = NULL;
	tree->count = 0;

	return tree;
}

T_NODE*
find_smallest_node
(T_NODE* root)
{
	if(root->left == NULL)
		return root;
	else
		return find_smallest_node(root->left);
}

T_NODE*
find_largest_node
(T_NODE* root)
{
	if(root->right == NULL)
		return root;
	else
		return find_largest_node(root->right);
}

T_NODE*
search_bst
(T_NODE* root, int key)
{
	if(root->data == key) // we found!
		return root;
	else if(key < root->data)
		return search_bst(root->left, key);
	else
		return search_bst(root->right, key);
	if(root == NULL){ // Not found!
		printf("Not found!\n");
		return NULL;
	}
}

T_NODE*					// Point is Recursive!!!
add_bst
(T_NODE* root, int data, int data1)
{
	if(root == NULL){ 			//leaf node
		T_NODE* newPtr = (T_NODE*)malloc(sizeof(T_NODE));
		if(!newPtr) return false;
		newPtr->left = NULL;
		newPtr->right= NULL;
		newPtr->data = data;			 // realy data insert
		newPtr->data1 = data1;					
		return newPtr;					 //newby
	}
	if(data < root->data){
		root->left = add_bst(root->left, data, data1);
		return root;
	} else{					// if, data == root->data?? -> nomally added 
		root->right = add_bst(root->right, data, data1);
		return root;
	}
}

void
preorder
(T_NODE* root)
{
	if(root != NULL){
		printf("%d ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

void
inorder
(T_NODE* root, int milk[], int unit[], int i)
{
	if(root != NULL){
		inorder(root->left, milk, unit, i);
//		printf("%d ", root->data);
		milk[i] = root->data;
		unit[i++] = root->data1;
		inorder(root->right, milk, unit, i);
	}
}

void
postorder
(T_NODE* root)
{
	if(root != NULL){
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->data);
	}
}

bool 
BST_insert
(BST_TREE* tree, int data, int data1)
{
	T_NODE* newRoot = add_bst(tree->root, data, data1);
	if(newRoot == NULL) return false;
	else {
		tree->root = newRoot;
		tree->count++;
		return tree;
	}
}

T_NODE*
del_bst		
(T_NODE* root, int data, bool* success)
{
	if(root == NULL){		// there is no delete_data
		*success = false;
		return NULL;
	}
	
	if(data < root->data){
		root->left = del_bst(root->left, data, success);
		return root;
	} else if(data > root->data){
		root->right = del_bst(root->right, data, success);
		return root;
	} else{					// matched!!
		T_NODE* del_node;
		if(root->left == NULL){
			del_node = root;
			root = root->right;
			free(del_node);
			*success = true;
			return root;
		} else if(root->right == NULL){
			del_node = root;
			root = root->left;
			free(del_node);
			*success = true;
			return root;
		} else{						// left, right 노드 다 존재 할때
			T_NODE* search;
			search = find_largest_node(root->left);		
//			search = root->left;					// when there is no find_ function 
//			while (search->right != NULL)
//				search = search->right;		
			root->data = search->data;
			root->left = del_bst(root->left, search->data, success);
			*success = true;
			return root;
		}
	}
}

bool
BST_del
(BST_TREE* tree, int data)
{
	bool success;
	T_NODE* newRoot;

	newRoot = del_bst(tree->root, data, &success);

	if(success){
		tree->root = newRoot;
		tree->count--;
		if(tree->count==0)
			tree->root = NULL;
	}
	return success;	
}
