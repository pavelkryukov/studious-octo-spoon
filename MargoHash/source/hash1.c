#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include "hash.h"
#define NUMBERCELL 20

struct ELEMENT{
	char* key;
	void* data;
	struct ELEMENT* next;
};

struct myTABLE{
	struct ELEMENT* CELL[NUMBERCELL];
};

TABLE* createTable(){
	struct myTABLE* t;
    t=malloc(sizeof(struct myTABLE));    
	if(t){
		int i;
		for(i=0;i<NUMBERCELL;i++) t->CELL[i]=NULL;
		return (TABLE*) t;
	}
	else{
		return NULL;
	}
}

int hash(char* key){
	int i,n=0;
	for(i=0;i<strlen(key);i++) n=n+*(key+i)+1;
	n=n % NUMBERCELL;
	return n;
}

int addEntry(TABLE* tab, char* key, void* data){
	struct ELEMENT* ptr;
	ptr=((struct myTABLE*) tab)->CELL[hash(key)];
	if(!ptr){
		ptr=malloc(sizeof(struct ELEMENT));
		((struct myTABLE*) tab)->CELL[hash(key)]=ptr;
		((struct myTABLE*) tab)->CELL[hash(key)]->next=NULL;
		((struct myTABLE*) tab)->CELL[hash(key)]->data=data;
        ((struct myTABLE*) tab)->CELL[hash(key)]->key =  malloc(sizeof(char)*(strlen(key)+1));
		strcpy(((struct myTABLE*) tab)->CELL[hash(key)]->key,key);
		return H_SUCCESS;
	}
	if(strcmp(ptr->key,key)==0) return H_KEYUSED;
	while(ptr->next){
		if(strcmp(ptr->next->key,key)==0) return H_KEYUSED;
		ptr=ptr->next;
	}
	ptr->next=malloc(sizeof(struct ELEMENT));
	ptr->next->next=NULL;
	ptr->next->data=data;
    ptr->next->key =  malloc(sizeof(char)*(strlen(key)+1));
	strcpy(ptr->next->key,key);
	return H_SUCCESS;
}

int removeEntry(TABLE* tab, char* key){
	struct ELEMENT* ptr;
	struct ELEMENT* ptr2;
	ptr=((struct myTABLE*) tab)->CELL[hash(key)];
	if(!ptr) return H_NOKEY;
	if(strcmp(ptr->key,key)==0){
		((struct myTABLE*) tab)->CELL[hash(key)]=ptr->next;
		free(ptr->key);
		free(ptr);
		return H_SUCCESS;
	}
	while(ptr->next){
		if(strcmp(ptr->next->key,key)==0){
			ptr2=ptr->next->next;
			free(ptr->next->key);
			free(ptr->next);
			ptr->next=ptr2;
			return H_SUCCESS;
		}
		ptr=ptr->next;
	}
	return H_NOKEY;
}

int getEntry(TABLE* tab, char* key, void** ptrdata){
	struct ELEMENT* ptr;
	ptr=((struct myTABLE*) tab)->CELL[hash(key)];
	if(!ptr) return H_NOKEY;
	if(strcmp(ptr->key,key)==0){
		*ptrdata=(ptr->data);
		return H_SUCCESS;
	}
	while(ptr->next){
		if(strcmp(ptr->next->key,key)==0){
			*ptrdata=(ptr->next->data);
			return H_SUCCESS;
		}
		ptr=ptr->next;
	}
	return H_NOKEY;
}

int deleteTable(TABLE* tab){
	int i;
	struct ELEMENT* ptr;
	struct ELEMENT* ptr2;
	for(i=0;i<NUMBERCELL;i++){
		ptr=((struct myTABLE*) tab)->CELL[i];
		while(ptr){
			ptr2=ptr->next;
			free(ptr->key);
			free(ptr);
			ptr=ptr2;
		}
	}
	free(tab);
	return 0;
}

