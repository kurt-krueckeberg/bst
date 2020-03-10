#include<malloc.h>
#include<stdbool.h>
#include<assert.h>

struct node {
	void * element;
	struct node * next;
};

typedef struct node * STACKPTR;

STACKPTR getNode(void * element){
	STACKPTR temp= (STACKPTR)malloc(sizeof(struct node));
	
	temp->element =element;
	temp->next=NULL;
	return temp;
}

void pushStack(STACKPTR * topRef, void * info){
	
	STACKPTR temp = getNode(info);
	if(*topRef==NULL){		
		temp->next=NULL;
		*topRef=temp;
		return;
	}
	temp->next=*topRef;
	*topRef=temp;
}


bool isEmptyStack(STACKPTR top){
	if(top == NULL){
		return true;
	}
	else 
		return false;
}

void * popStack(STACKPTR * topRef){
	if(!isEmptyStack(*topRef)){
		STACKPTR temp = *topRef;
		*topRef = temp->next;
		temp->next=NULL;
		return temp->element;
	}
	assert(0);
}
