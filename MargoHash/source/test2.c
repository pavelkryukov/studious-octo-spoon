#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "hash.h"

int test1()
{
	TABLE* a;
	int i;
	void * pointer;
	char* str;
	char* data;
	
	a=createTable();
	
	str=(char*)malloc(5*sizeof(char));
	data=(char*)malloc(5*sizeof(char));
	
	for(i=0;i<4;i++)
	{
		str[i]='a'+i;
		data[i]='s'+i;
	}
		
	if(addEntry(a,str,(void*)data)!=H_SUCCESS)
	{
		printf("Невозможно добавить ячейку\n");
		return 1;
	}
	
	if(getEntry(a,str,&pointer)!=H_SUCCESS)
	{
		printf("Невозможно получить data (1)\n");
		return 1;
	}
	
	if(strcmp(data,pointer))
	{
		printf("Не совпадает записываемая и извлекаемая информация в ячейке\n"); 
		return 1;
	}
	
	if(getEntry(a,str,&pointer)!=H_SUCCESS)
	{
		printf("Невозможно получить data (2)\n");
		return 1;
	}
	
	if(strcmp(data,pointer))
	{
		printf("Не совпадает записываемая и извлекаемая информация в ячейке\n"); 
		return 1;
	}
	
	if(deleteTable(a)!=H_SUCCESS)
	{
		printf("Невозможно удалить таблицу\n"); 
		return 1;
	}
	
	free(str);
	free(data);
	
	return H_SUCCESS;
}

int test2()
{
	TABLE* a;
	TABLE* b;
	int i;
	void * pointer1;
	void * pointer2;
	char* str;
	char* data;
	
	str=(char*)malloc(256*sizeof(char));
	data=(char*)malloc(256*sizeof(char));
	
	a=createTable();
	b=createTable();
	
	for(i=0;i<255;i++)
	{
		str[i]='a'+i%25;
		data[i]='s'+i%25;
	}
	
	if(addEntry(a,str,(void*)data)!=H_SUCCESS)
	{
		printf("Невозможно добавить ячейку\n");
		return 1;
	}
	
	if(addEntry(b,str,(void*)data)!=H_SUCCESS)
	{
		printf("Невозможно добавить ячейку\n");
		return 1;
	}
	
	if(getEntry(a,str,&pointer1)!=H_SUCCESS)
	{
		printf("Невозможно получить data (1)\n");
		return 1;
	}
	
	if(getEntry(b,str,&pointer2)!=H_SUCCESS)
	{
		printf("Невозможно получить data (2)\n");
		return 1;
	}
	
	if(strcmp(pointer1,pointer2))
	{
		printf("Не совпадает записываемая и извлекаемая информация в ячейке\n"); 
		return 1;
	}
	
	if(removeEntry(a,str)!=H_SUCCESS)
	{
		printf("Ошибка в удалении ячейки\n");
		return 1;
	}
	
	if(removeEntry(a,str)!=H_NOKEY)
	{
		printf("Ошибка в удалении ячейки\n");
		return 1;
	}
	
	if(getEntry(a,str,&pointer1)!=H_NOKEY)
	{
		printf("Найден удаленный элемент\n");
		return 1;
	}

	if(getEntry(b,str,&pointer2)!=H_SUCCESS)
	{
		printf("Невозможно получить data (2)\n");
		return 1;
	}
	
	if(strcmp(data,pointer2))
	{
		printf("Не совпадает записываемая и извлекаемая информация в ячейке\n"); 
		return 1;
	}
	
	if(deleteTable(a)!=H_SUCCESS)
	{
		printf("Невозможно удалить таблицу\n"); 
		return 1;
	}
	
	if(deleteTable(b)!=H_SUCCESS)
	{
		printf("Невозможно удалить таблицу\n"); 
		return 1;
	}
	
	free(str);
	free(data);
	
	return H_SUCCESS;
}

int test3()
{
	TABLE* a;
	int i;
	void * pointer;
	char** t;
	
	a=createTable();
	
	t=(char**)malloc(1000*sizeof(char*));
	
	for(i=0; i<1000; i++)
	{
		t[i]=(char*)malloc(5*sizeof(char));
		
		t[i][0]='a';
		t[i][1]='a'+i/625;
		t[i][2]='a'+i/25;
		t[i][3]='a'+i%25;
		
		
		if(addEntry(a,t[i],(void*)t[i])!=H_SUCCESS)
		{
			printf("Невозможно добавить ячейку %d\n",i);
			return 1;
		}
	}
	
	for(i=0; i<1000; i++)
	{
		if(getEntry(a,t[i],&pointer)!=H_SUCCESS)
		{
			printf("Невозможно получить data %d ячейки\n",i);
			return 1;
		}
	
		if(strcmp(t[i],pointer))
		{
			printf("Не совпадает записываемая и извлекаемая информация в ячейке %d\n",i); 
			return 1;
		}
		
	}
	
	for(i=0; i<1000; i++)
	{
		if(removeEntry(a,t[i])!=H_SUCCESS)
		{
			printf("Ошибка в удалении ячейки %d\n",i);
			return 1;
		}
	}
	
	for(i=0; i<1000; i++)
	{
		if(getEntry(a,t[i],&pointer)!=H_NOKEY)
		{
			printf("Найден удаленный элемент в %d ячейке \n",i);
			return 1;
		}
	}
	
	for(i=0; i<1000; i++)
	{
		free(t[i]);
	}
	
	free(t);
	
	if(deleteTable(a)!=H_SUCCESS)
	{
		printf("Невозможно удалить таблицу\n"); 
		return 1;
	}
	
	return H_SUCCESS;
} 

int main()
{
	if(test1()==H_SUCCESS)
	{
		printf("Тест 1 пройден успешно\n");
	}
	else
	{
		printf("Тест 1 не пройден\n");
	}
	
	if(test2()==H_SUCCESS)
	{
		printf("Тест 2 пройден успешно\n");
	}
	else
	{
		printf("Тест 2 не пройден\n");
	}
	
	if(test3()==H_SUCCESS)
	{
		printf("Тест 3 пройден успешно\n");
	}
	else
	{
		printf("Тест 3 не пройден\n");
	}
	
	return 0;
}
