#include <stdio.h>
#include "hash.h"
#include <string.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#define HASHSIZE 10000
#define H_OVERLOAD -5

struct ITEM{
  char status; //0 means not used, 1 is used, 2 is deleted
  char* item_key;
  void* item_ptr;
};

struct MyTable{
  struct ITEM* table;
  int items;
  int hc;
};

TABLE* createTable() {
  struct MyTable* ptr;
  ptr = calloc(1,sizeof(struct MyTable));
  ptr->table = (struct ITEM*)calloc(HASHSIZE,sizeof(struct ITEM));
  return (TABLE*)ptr; //ptr is NULL if there was a mistake
}

int GetHashCode(char *s)
        {
		int i;
                int num = 0;
		for (i = strlen(s); i > 0; i--) num+=s[i];
                int num2 = num;
		for (i = strlen(s); i > 0; i -= 4)
                {
                    num = (((num << 1) + num) + (num >> 1));
                    if (i <= 2) break;
                    
                    num2 = (((num2 << 1) + num2) + (num2 >> 1));
                }
                return abs((num + (num2 * 141))%HASHSIZE);
        }

 //* Н_SUCCESS — успешный результат.
 //* Н_MEMERROR — ошибка памяти.
 //* Н_KEYUSED — ключ уже использовался
// * Н_NOTABLE — таблицы по адресу не существует либо адрес неверный.
 
int setinfo(struct ITEM* item, char* info_key, void* info_ptr){
  item->status=1;
  item->item_ptr=info_ptr;
  item->item_key=(char*)malloc(sizeof(char)*(strlen(info_key)+1));
    if (item->item_key==NULL) return H_MEMERROR;
  strcpy(item->item_key, info_key);
  return H_SUCCESS;
}

int GetHashCode2(int hcode){
    return (hcode + 77)%HASHSIZE;
}

int addEntry(TABLE* table, char* info_key, void* info_ptr){ 
  struct MyTable* p = (struct MyTable*)table;
  if (p->items==HASHSIZE) return H_OVERLOAD;
  int hcode=GetHashCode(info_key);//got hashcode
  while (((p->table)+hcode)->status==1) {
    if(!strcmp(((p->table)+hcode)->item_key, info_key)) return H_KEYUSED;
    hcode=GetHashCode2(hcode);
  }
    if (setinfo((p->table)+hcode, info_key, info_ptr)==H_MEMERROR) return H_MEMERROR;
    (p->items)++;    
  return H_SUCCESS;
}

int getEntry(TABLE* table, char* info_key, void** out_ptr){
  int hcode=GetHashCode(info_key);
  int n=0;
  struct MyTable* p = (struct MyTable*)table;
  while((((p->table)+hcode)->status!=0)||(n==HASHSIZE)){
    if ((((p->table)+hcode)->status==1)&&(!strcmp(((p->table)+hcode)->item_key,info_key))) {
      *out_ptr=((p->table)+hcode)->item_ptr;
      p->hc=hcode;
      return H_SUCCESS;
    }
    hcode=GetHashCode2(hcode);
    n++;
  }
  return H_NOKEY;
}

int removeEntry( TABLE* table, char* info_key){
  void* ptr=NULL;
  if(getEntry(table, info_key, &ptr)==H_NOKEY) return H_NOKEY;
  struct MyTable* p = (struct MyTable*)table;
  free(((p->table)+(p->hc))->item_key);
  ((p->table)+(p->hc))->status=2;
  (p->items)--; 
  return H_SUCCESS;
}

int deleteTable( TABLE* table){
  int i;
  struct MyTable* p = (struct MyTable*)table;
  for(i=0; i<HASHSIZE; i++){
    if (((p->table)+i)->status==1) free(((p->table)+i)->item_key);
  }
  free(p->table);
  free(table);
  return H_SUCCESS;
}