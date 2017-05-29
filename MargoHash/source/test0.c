#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "hash.h"
#include <stdlib.h>

struct ITEM{
  char status; //0 means not used, 1 is used, 2 is deleted
  char* item_key;
  void* item_ptr;
};


#define H_OVERLOAD -5
int main(){
  TABLE* table=createTable();
  if(table==NULL) {printf("Error in createTable\n"); goto test_end;}
  char key[10];  
      if(errno!=0) printf("Error while opening text.txt:%s\n", (char*)strerror(errno));
  int i, result;
  
 

//printf("%s\n",((struct ITEM*)table+6409)->item_key);  
  
  
//testing addEntry  
  printf("testing addEntry...\n");
  for(i=0; i<1000; i++){
  memset(key,0,10);
  snprintf(key,10,"%d%d",i,i);
  result=addEntry(table, key, NULL);
    switch(result) {
      case H_OVERLOAD: printf("Table is overloaded, test failed\n"); goto end1;
      case H_MEMERROR: printf("Memory error, test failed\n"); goto end1;
      case H_KEYUSED: //printf("Key has been used alredy, that's ok\n"); 
		      break;
      default: break;
    }
  }
    
printf("addEntry is ok\n\n");
end1:

      if(errno!=0) printf("Error while opening text.txt:%s\n", (char*)strerror(errno)); 
//testing getEntry
printf("testing getEntry...\n");
for(i=0; i<1000; i++){
  memset(key,0,10);  
  snprintf(key,10,"%d%d",i,i);
  void* ptrtest;
  result=getEntry((TABLE*)table, key, &ptrtest);
    switch(result) {
      case H_NOKEY: printf("added key wasn't found - fail\n"); goto end2;
      default: break;
    }
}
printf("getEntry is ok\n\n");
end2:

//testing removeEntry
      if(errno!=0) printf("Error while opening text.txt:%s\n", (char*)strerror(errno)); 

printf("testing removeEntry...\n");

for(i=0; i<1000; i++){
  memset(key,0,10);  
  snprintf(key,10,"%d%d",i,i);
  result=removeEntry(table, key);
    switch(result) {
      case H_NOKEY: //printf("such key was already deleted\n"); 
		    break;
      default: break;
    }
}

for(i=1000; i<1100; i++){
  memset(key,0,10);  
  snprintf(key,10,"%d%d",i,i);
  result=removeEntry(table, key);
    switch(result) {
      case H_NOKEY: break;
      default: printf("It musn't be here. removeEntry works badly\n"); goto end3;
    }
}
printf("removeEntry is ok\n");
end3:
test_end:
deleteTable(table);
  return 0;
}
