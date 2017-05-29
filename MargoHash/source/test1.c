#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include "hash.h"

TABLE* a;
TABLE* b;
TABLE* c;
char* keys;
char* s;
char* sc;
int i;
int* j;

int test1(){
	for(i=1;i<1000000;i++){
		a=createTable();
		if((!a)||(a<(TABLE*)NULL)){
			//printf("test1\nne sozdal hash nomer %d\n",i);
			return 1;
		}
		if(addEntry(a,keys,"abd")!=H_SUCCESS){
			//printf("test1\nne dobavil stroku nomer %d\n");
			return 1;
		}
		if(deleteTable(a)){
			//printf("test1\nne udalen hash nomer %d\n",i);
			return 1;
		}
	}
	return 0;
}

int test2(){
	for(i=1;i<1000000;i++){
		a=createTable();
		if((!a)||(a<(TABLE*)NULL)){
			//printf("test2\nne sozdal hash nomer %d\n",i);
			return 1;
		}
		if(addEntry(a,keys,a)!=H_SUCCESS){
			//printf("test2\nne dobavil tablicu nomer %d\n");
			return 1;
		}
		if(deleteTable(a)){
			//printf("test2\nne udalen hash nomer %d\n",i);
			return 1;
		}
	}
	return 0;
}

int test3(){
	a=createTable();
	if((!a)||(a<(TABLE*)NULL)){
		//printf("test3\nne sozdal hash a\n");
		return 1;
	}
	b=createTable();
	if((!b)||(b<(TABLE*)NULL)){
		//printf("test3\nne sozdal hash b\n");
		return 1;
	}
	if(addEntry(b,keys,a)!=H_SUCCESS){
		//printf("test3\nne dobavil tablicu\n");
		return 1;
	}
	if(addEntry(b,keys,a)!=H_KEYUSED){
		//printf("test3\ndobavil tablicu povtorno\n");
		return 1;
	}
	if(getEntry(b,keys,(void**) &c)!=H_SUCCESS){
		//printf("test3\nne nashel tablicu\n");
		return 1;
	}
	if(a!=c){
		//printf("test3\nnepravilno dostal tablicu\n");
		return 1;
	}
	strcpy(keys,"abd");
	if(getEntry(b,keys,(void**) &c)!=H_NOKEY){
		//printf("test3\nnashel tablicu po nesuschestvueschemu kluchu\n");
		return 1;
	}
	if(removeEntry(b,keys)!=H_NOKEY){
		//printf("test3\nudalil tablicu po nesuschestvueschemu kluchu\n");
		return 1;
	}
	strcpy(keys,"abc");
	if(removeEntry(b,keys)!=H_SUCCESS){
		//printf("test3\nne udalil tablicu\n");
		return 1;
	}
	if(removeEntry(b,keys)!=H_NOKEY){
		//printf("test3\nudalil tablicu povtorno\n");
		return 1;
	}
	if(getEntry(b,keys,(void**) &c)!=H_NOKEY){
		//printf("test 3\nnashel tablicu\n");
		return 1;
	}
	if(deleteTable(b)!=H_SUCCESS){
		//printf("test3\nne udalen hash b\n",i);
		return 1;
	}
	if(deleteTable(a)!=H_SUCCESS){
		//printf("test3\nne udalen hash a\n",i);
		return 1;
	}
	return 0;
}

int test4(){
	a=createTable();
	if((!a)||(a<(TABLE*)NULL)){
		//printf("test4\nne sozdal hash a\n");
		return 1;
	}
	if(addEntry(a,keys,s)!=H_SUCCESS){
		//printf("test4\nne dobavil stroku\n");
		return 1;
	}
	if(addEntry(a,keys,s)!=H_KEYUSED){
		//printf("test4\ndobavil stroku povtorno\n");
		return 1;
	}
	if(getEntry(a,keys,(void**) &sc)!=H_SUCCESS){
		//printf("test4\nne nashel stroku\n");
		return 1;
	}
	if(s!=sc){
		//printf("test4\nnepravilno dostal stroku\n");
		return 1;
	}
	strcpy(keys,"abd");
	if(getEntry(a,keys,(void**) &sc)!=H_NOKEY){
		//printf("test4\nnashel stroku po nesuschestvueschemu kluchu\n");
		return 1;
	}
	if(removeEntry(a,keys)!=H_NOKEY){
		//printf("test4\nudalil stroku po nesuschestvueschemu kluchu\n");
		return 1;
	}
	strcpy(keys,"abc");
	if(removeEntry(a,keys)!=H_SUCCESS){
		//printf("test4\nne udalil stroku\n");
		return 1;
	}
	if(removeEntry(a,keys)!=H_NOKEY){
		//printf("test4\nudalil stroku povtorno\n");
		return 1;
	}
	if(getEntry(a,keys,(void**) &sc)!=H_NOKEY){
		//printf("test 4\nnashel stroku\n");
		return 1;
	}
	if(deleteTable(a)!=H_SUCCESS){
		//printf("test4\nne udalen hash a\n",i);
		return 1;
	}
	return 0;
}

int test5(){
	a=createTable();
	if((!a)||(a<(TABLE*)NULL)){
		//printf("test5\nne sozdal hash a\n");
		return 1;
	}
	if(addEntry(a,keys,j)!=H_SUCCESS){
		//printf("test5\nne dobavil chislo\n");
		return 1;
	}
	if(addEntry(a,keys,j)!=H_KEYUSED){
		//printf("test5\ndobavil chislo povtorno\n");
		return 1;
	}
	if(getEntry(a,keys,(void**) &j)!=H_SUCCESS){
		//printf("test5\nne nashel chislo\n");
		return 1;
	}
	if(13!=*j){
		//printf("test5\nnepravilno dostal chislo\n");
		return 1;
	}
	strcpy(keys,"abd");
	if(getEntry(a,keys,(void**) &i)!=H_NOKEY){
		//printf("test5\nnashel chislo po nesuschestvueschemu kluchu\n");
		return 1;
	}
	if(removeEntry(a,keys)!=H_NOKEY){
		//printf("test5\nudalil chislo po nesuschestvueschemu kluchu\n");
		return 1;
	}
	strcpy(keys,"abc");
	if(removeEntry(a,keys)!=H_SUCCESS){
		//printf("test5\nne udalil chislo\n");
		return 1;
	}
	if(removeEntry(a,keys)!=H_NOKEY){
		//printf("test5\nudalil chislo povtorno\n");
		return 1;
	}
	if(getEntry(a,keys,(void**) &i)!=H_NOKEY){
		//printf("test 5\nnashel chislo\n");
		return 1;
	}
	if(deleteTable(a)!=H_SUCCESS){
		//printf("test5\nne udalen hash a\n",i);
		return 1;
	}
	return 0;
}

int main(){
	keys=malloc(4);
	s=malloc(4);
	j=malloc(4);
	strcpy(keys,"abc");
	strcpy(s,"def");
	*j=13;

	if(test1()!=0) printf("test1\t\t -\n");
	else printf("test1\t\tOK\n");

	if(test2()!=0) printf("test2\t\t -\n");
	else printf("test2\t\tOK\n");
	
	if(test3()!=0) printf("test3\t\t -\n");
	else printf("test3\t\tOK\n");

	if(test4()!=0) printf("test4\t\t -\n");
	else printf("test4\t\tOK\n");

	if(test5()!=0) printf("test5\t\t -\n");
	else printf("test5\t\tOK\n");

	free(keys);
	free(s);
	free(j);
	
	return 0;
}
