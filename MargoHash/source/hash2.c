#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
   
#define MULTIPLIER 31
#define hashSize 256

typedef struct Entry Entry;

struct Entry{
	
	char *name;
    Entry *next;         
    void* data;
                         
};


TABLE* createTable()
{
	TABLE* hashTable;
	int i;
	
	if ((hashTable = malloc(hashSize * sizeof(Entry*))) == NULL) 
	{
        return NULL;
    }
    
    for(i=0; i<hashSize; i++)
	{
		hashTable[i] = NULL;
	}
    
    return hashTable;
}


unsigned int hash(char* str) 
{
	unsigned int h;
	unsigned char *p;
	
	h=0;
	
	for( p=(unsigned char*)str; *p!='\0'; p++)
	{
	  h = MULTIPLIER * h + *p;
	} 
	
    return (h % hashSize);
}

int addEntry(TABLE* hashTable, char* name, void* data) 
{
    unsigned int h;
	Entry *sym;

    h=hash(name);
    
    for (sym=hashTable[h]; sym!=NULL; sym=sym->next)
      if (strcmp(sym->name,name)==0)
      {
		return H_KEYUSED;
	  }
		
    if((sym=(Entry* )malloc(sizeof(Entry)))==NULL)
    {
      return H_MEMERROR;
    }
    
    if((sym->name=malloc(sizeof(char)*strlen(name)))==NULL)
    {
		return H_MEMERROR;
	}
	
    strcpy(sym->name,name);
    sym->data=data;
    sym->next=hashTable[h];
    hashTable[h]=sym;
    
    return H_SUCCESS;
}

int getEntry (TABLE* hashTable, char* name, void** data) 
{
   unsigned int h;
   Entry *sym;
	
    h=hash(name);
    
    for (sym=hashTable[h]; sym!=NULL; sym=sym->next)
    {
      if (strcmp(sym->name,name)==0)
      {
		*data=sym->data;
		return H_SUCCESS;
      }
    }
    
    return H_NOKEY;
}

int removeEntry(TABLE* hashTable, char* name)
{
    Entry *sym0, *sym;
    unsigned int h;

    sym0 = 0;
    h = hash(name);
    sym = hashTable[h];
    
    while (sym && strcmp(sym->name, name)) 
    {
        sym0 = sym;
        sym = sym->next;
    }
    
    if (!sym)
    {
		 return H_NOKEY;
	}
    
    if (sym0)
    {
        sym0->next = sym->next;
    }
    
    else
    {
        hashTable[h] = sym->next;
    }   
    
    free (sym->name);
    free (sym);
    
    return H_SUCCESS;
}

int deleteTable(TABLE* hashTable)
{
	unsigned int i;
	Entry *sym, *sym0;
	
	for(i=0; i<hashSize; i++)
	{
		sym=hashTable[i];
		
		while(sym!=NULL)
		{
			sym0=sym;
			sym=sym->next;
            free(sym0->name);
			free(sym0);
		}
	}
	
	free(hashTable);
	return H_SUCCESS;
}

