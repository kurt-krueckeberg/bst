#include<stdio.h>
#include"Tree.h"


int main(int length, char * argv[]){
	
	TREEPTR treeHead = NULL;
	int i = 0;
	int element[]={100,10,200,12,20,1,4,3,67,66,210,199};
	while(i < 12){
		addToTree(&treeHead , element[i]);
		i++;
	}
	printf("\n Printing Inorder recursive");
	inOrderRec(treeHead);
	printf("\n Printing Inorder Iterative");
	inOrderIterative(treeHead);
	
	printf("\n Printing Preorder recursive");
	preOrderRec(treeHead);
	printf("\n Printing Preorder Iterative");
	preOrderIterative(treeHead);	
	
	printf("\n Printing Postorder recursive");
	postOrderRec(treeHead);
	printf("\n Printing Postorder Iterative");
	postOrderIterative(treeHead);
}

