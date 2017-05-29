/*
 * hash3.c
 *
 * ���������� ������������� ���-�������.
 *
 * (C) 2011 ������ �����, ��. 913.
*/

#include <stdlib.h> // malloc, free
#include <string.h> // strcpy, strcmp

#include "hash.h"

#define TABLE_SIZE 269 // ������ ���-�������, ������ ������� ������.

/*
 * ���������, ��������������� ������ ���-�������
*/
typedef struct ENTRY
{
    char* key; // ����
    void* data; // ��������� �� ������
    struct ENTRY* next; // ������������ ����������� ������
    struct ENTRY* prev;
} ENTRY;

/*
 * ���-�������.
 * ��������� ������, ��������� � ����� � �������� � ������ TABLE_SIZE.
 * ���������� ��� �� 0 �� TABLE_SIZE-1.
*/
#define K1 3
#define K2 7
int hash(char *key)
{
    int i = 0;
    int value = 0;
    int len = strlen(key);
    for( i = 0; i < len; i++)
    {
        value = ( K1 * ( value + K2 * key[i])) % TABLE_SIZE;
    }
    return value;
}

/*
 * ����������� �������.
 * ��������� ���� � ������.
 * ���������� ��������� �� ������.
 * ���� ������ ���������� ��������, ���������� NULL.
*/
ENTRY* initEntry( char* key, void* data)
{
    ENTRY* record;      
    record = malloc(sizeof(ENTRY)); // �������������� ����� ��� ������.  
    if (!record)
    {
        return NULL;
    }
    record->key = malloc(sizeof(char)*strlen(key));  
    if (!record->key)
    {
        return NULL;
    }
    strcpy( record->key, key);      // �������� ����.
    record->data = data;            // �������� ����������.   
    record->next = NULL;            // �������� �������� ���������� ��������.
    record->prev = NULL;
    return record;
}


/*
 * ���������� �������.
 * ��������� ����� ������.
*/
void destroyEntry( ENTRY* victim)
{
    victim->prev = NULL;
    victim->next = NULL;
    free(victim->key);
    free(victim);
}
    
/*
 * ����� �������� � ������ � �������� ������
 * ��������� ����� ������ ������ (where) � ����.
 * ���������� ����� �������� �������� ��� NULL, ���� ������� �� ������.
*/
ENTRY* listSearch( ENTRY* where, char* key)
{
    if (!strcmp(where->key, key))
    {     
    // ���� ������� � ������� ������ ��� ���� � �������, ������� ��� �����.
        return where;
    }
    else
    {
        if (where->next)
        {
        // ���������� ��������� � ����� ������, ������� �������� ������� ������.
            return listSearch( where->next, key);
        }
        else
        {
        // ���� �� ����� �� �����, ���������� NULL.
            return NULL;
        }
    }
}

/*
 * ����������� ����������� ������.
 * ��������� ����� ������� �������� ������.
 */
void listDelete(ENTRY* what)
{
    if (what)
    {
    // ���� ������� �� ������, �� ������� ������� ���������.
    // �������� ������� � ���� ������ � ����� ����������� � �����.    
        listDelete(what->next);
        destroyEntry(what);
    }
}

/*
 * �������� �������.
 * ��� ������� ���������� NULL, ����� � ����� �� �������.
*/
TABLE* createTable()
{
    ENTRY** book;
    book = malloc(sizeof(ENTRY*) * TABLE_SIZE); // �������������� ������ ����������
    if (book)
    { 
    // ���� ������ ����������, �� ��������� ������ ������ � ���������� � ���� ��������� �� �������.
        int i;
        for ( i = 0; i < TABLE_SIZE; i++)
        {
            book[i] = NULL;
        }
        return (TABLE*)book;        
    }
    return NULL;
}

/*
 * ���������� ������.
 * ��������� ����� �������, ���� � ������ �� ������.
 * � ������ ������ ���������� H_SUCCESS.
 * ���� ��������� ������ ��������� ������, ���������� H_MEMERROR.
 * ���� ������ � �������� ������ ��� ����������, ���������� H_KEYUSED.
*/
int addEntry( TABLE* book, char* key, void* data)
{
    ENTRY** myBook = (ENTRY**)book; // �������������� ������������ ���� � ��������������
    int i = hash(key);  // ��������� ��� ����� 
    if (!myBook[i])     
    {
    // ���� ������ �����, ���������� ������� ������ � ���.
        ENTRY *record = initEntry( key, data);          
        if (record)
        {
            myBook[i] = record;
            return H_SUCCESS;
        }
        else
        {
        // ���� ������ �� ����������, ��������.        
            return H_MEMERROR;
        }
    }
    else
    {     
    // ���� ������ �� �����, ��������� �� ��������� ������������� �����.
        if (listSearch(myBook[i], key))       
        {
        // ���� ���� ��� �����������, �������� �� ����.
            return H_KEYUSED;
        }
        else
        { 
        // ���� ���� �� �����������, ��������� ������ � ������ ������.
            ENTRY *record = initEntry( key, data);             
            if (record)
            {
                record->next = myBook[i];
                myBook[i]->prev = record;
                myBook[i] = record;
                return H_SUCCESS;
            }
            else
            {
            // ���� ������ �� ����������, ��������.
                return H_MEMERROR;
            }
        }
    }
}

/*
 * ����� ������.
 * ��������� ����� ������� � ����.
 * � ������ ������ ���������� H_SUCCESS � ���������� � *data ��������� �� ������.
 * ���� ����� ��� ������������, ���������� H_NOKEY � ���������� � *data NULL
*/
int getEntry( TABLE* book, char* key, void** data)
{
    ENTRY** myBook = (ENTRY**)book; // �������������� ������������ ���� � ��������������.
    int i = hash(key);  // ��������� ��� �����. 
    if (!myBook[i]) 
    {
    // ���� ���� ����� � ������������ �� ���������� �� ���� ������, �������� � ���, ��� �������� ����� �� ������������� ������.
        *data = NULL;
        return H_NOKEY;
    }
    else
    {
    // ���� ���� ���� �� ���� ������, ������, ������ �� ����.
    // ���������� ����� �� ������.
        ENTRY* founded = listSearch( myBook[i], key);
        if (founded)
        {
        // ���� ���-���� �������, ������ ��������� �� ���������
            *data = founded->data;
            return H_SUCCESS;
        }
        else
        {
        // ���� ������ �� �������, ��������.
            *data = NULL;  
            return H_NOKEY;
        }
    }
}

/*
 * �������� ������.
 * ��������� ������ �� ������� � ����.
 * ���� ������� ������ � ������, ���������� H_SUCCESS.
 * ���� ������� �� ������, ���������� H_NOKEY.
 */
int removeEntry( TABLE* book, char* key)
{
    ENTRY** myBook = (ENTRY**)book; // �������������� ������������ ���� � ��������������.
    int i = hash(key);  // ��������� ��� �����. 
    if (!myBook[i]) 
    {
    // ���� ���� ����� � ������������ �� ���������� �� ���� ������, �������� � ���, ��� �������� ����� �� ������������� ������.
        return H_NOKEY;
    }
    else
    {
    // ���� ���� ���� �� ���� ������, ������, ������ �� ����.
    // ���������� ����� �� ������.
        ENTRY* victim = listSearch( myBook[i], key);
        if (victim)
        {
        // ���� ���-���� �������, ������� �������.
            if (victim->next)
            {
            // �������� ����������, ��� ����� ���� ������ �������.
                victim->next->prev=victim->prev;
            }
            if (victim->prev)
            {
            // �������� �����������, ��� ������� ���� ������ �������.
                victim->prev->next=victim->next;
            }
            else
            {
            // ���� ������� �������� ������, �������� ������ � �������
                myBook[i] = victim->next;
            }
            // ����������� ������.
            destroyEntry(victim);
            return H_SUCCESS;            
        }
        else
        {
        // ���� ������ �� �������, ��������.
            return H_NOKEY;
        }
    }
}

/* 
 * �������� ���-�������.
 * ��������� ����� �������.
 * ���� �������, �� ��������� H_SUCCESS.
*/
int deleteTable( TABLE* book)
{
    ENTRY** myBook = (ENTRY**)book; // �������������� ������������ ���� � ��������������.
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        if (myBook[i])
        {
            listDelete(myBook[i]);
        }
    }
    free(myBook);
    return H_SUCCESS;
}