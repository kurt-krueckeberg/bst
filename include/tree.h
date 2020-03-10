#include<malloc.h>
#include <stdbool.h>
#include<assert.h>
#include"Stack.h"

struct treeNode{
	int info;
	bool isVisited;
	struct treeNode * left;
	struct treeNode * right;
};

typedef struct treeNode * TREEPTR;

TREEPTR getTreeNode(int x){
	TREEPTR temp = (TREEPTR)malloc(sizeof(struct treeNode));
	temp->info=x;
	temp->left = temp->right=NULL;
	temp->isVisited=false;
	return temp;
}

void addToTree(TREEPTR * treeRef, int x){	
	
	if((*treeRef) == NULL){
		TREEPTR temp = getTreeNode(x);		
		*treeRef = temp;
		return;
	}
	if(x < (*treeRef)->info ){
		addToTree(&(*treeRef)->left,x);
	}else{
		addToTree(&(*treeRef)->right,x);
	}
}



void inOrderRec(TREEPTR tree){
	if(tree!=NULL){		
		inOrderRec(tree->left);
		printf("\n %d",tree->info);
		inOrderRec(tree->right);
	}	
}

void preOrderRec(TREEPTR tree){
	if(tree!=NULL){
		printf("\n %d",tree->info);
		preOrderRec(tree->left);		
		preOrderRec(tree->right);
	}	
}

void postOrderRec(TREEPTR tree){
	if(tree!=NULL){
		postOrderRec(tree->left);		
		postOrderRec(tree->right);
		printf("\n %d",tree->info);
		
	}	
}

void inOrderIterative(TREEPTR tree){
	
	STACKPTR stackTop = NULL;
	
	TREEPTR current = tree;

	do{
		while(current!=NULL){
			pushStack(&stackTop,(void *)current);
			current=current->left;
		}
		
		if(!isEmptyStack(stackTop)){
			current = popStack(&stackTop);
			printf("\n%d", current->info);
			current=current->right;
		}
	}while(!isEmptyStack(stackTop) || current!=NULL);
}


void preOrderIterative(TREEPTR tree){
	
	STACKPTR stackTop = NULL;
	
	TREEPTR current = tree;

	do{
		while(current!=NULL){
			printf("\n%d", current->info);
			pushStack(&stackTop,(void *)current->right);
			current=current->left;
		}
		
		if(!isEmptyStack(stackTop)){
			current = popStack(&stackTop);		
		}
	}while(!isEmptyStack(stackTop) || current!=NULL);
}


void postOrderIterative(TREEPTR tree){
	
	STACKPTR stackTop = NULL;
	
	TREEPTR current = tree;
	
	do{
		while(current!=NULL){
			pushStack(&stackTop,current);
			current=current->left;
		}
		if(!isEmptyStack(stackTop)){
			current=popStack(&stackTop);
			if(current->right==NULL){
				printf("\n%d",current->info);
				current=current->right;
			}
			else{
				if(current->isVisited==true){
					printf("\n%d",current->info);
					current=NULL;
				}else{
					current->isVisited=true;
					pushStack(&stackTop,current);
					current=current->right;
				}
			}
		}
	}while(!isEmptyStack(stackTop) || current!=NULL);
		
}