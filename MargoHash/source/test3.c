/* 
 * test3.c
 * 
 * ����� ������ ������������� ���-�������.
 *
 * (C) 2011 ������ �����, ��. 913.
*/

#include <stdio.h>     // printf, snprintf
#include <stdlib.h>    // rand, srand
#include <string.h>    // strcpy, strcmp
#include <time.h>      // time

#include "hash.h"     // ���������� ���-�������

#define TEST_SUCCESS 0  // �������� ���������� �����
#define TEST_FAIL 1     // ���� �� ������� (������ � ����������)
#define TEST_ERROR 2    // �� ����� ����� ��������� ������

#define KEY_LEN 4      // ����� ���������� �����, ������������� ��� �����
#define KEY_LETTERS 20 // ���������� ���� ASCII, ������������ ��� �������� ���������� �����
#define MAX_RND 64     // ����������� ��������� ��������� �����
#define KEY_MAXLEN 20  // ����������� ��������� ����� ���������������� �� ������� �����

/*
 * ��������� �����
 * ��������� ����� �������, ����������� ���� � �������� �����.
 * ��� ������� ���������� ����� ���������� 0, ����� � 1.
*/
int test(int (*func)(), char* name)
{
    printf(">> %s: started\n", name);
    int result = func();
    switch ( result)
    {
        case TEST_SUCCESS: 
            printf(">> %s: succeeded\n\n", name); 
            break;
        case TEST_FAIL:
            printf(">> %s: failed\n\n", name); 
            break;
        case TEST_ERROR: 
            printf(">> %s: internal error\n\n", name); 
            break;
    }
    return result;
}

/*
 * �������� ���������� ����� ����� KEY_LEN
 * ���� ������ �� ����������, �� ������������ NULL.
*/
char* randKey()
{
    char* buffer = malloc(sizeof(char) * KEY_LEN);
    if (!buffer)
    {
        return NULL;
    }
    int i;
    for (i = 0; i < KEY_LEN; i++)
    {
        buffer[i] = 'a' + rand() % KEY_LETTERS;
    }
    return buffer;
}

/*
 * ����� �� ��������� ����� ����������.
 * ������������ ��������: 
 *    TEST_SUCCESS 0  � �������� ���������� �����
 *    TEST_FAIL 1     � ���� �� ������� (������ � ����������)
 *    TEST_ERROR 2    � �� ����� ����� ��������� ������
*/

/*
 * ������� ������� ������ � ���-�������, ���������� �����.
 * ��������� ��������� ������ � ������� addEntry.
*/
int test_add(void)
{
    // ���������� �������
    int random_number = rand() % MAX_RND;
    printf("The number is %d\n", random_number);
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }
    printf("The key is '%s'\n", random_key);
    
    // �������� ������� �������. ���� ���-�� ���������� �������, �������.
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
    
    // �������� ������� ���������� ������.
    int result = addEntry( book, random_key, (void*)random_number); 
     
    switch (result)
    {
        case H_SUCCESS:
        // ���� ������ ����������, �������� �� ������
            printf("Success\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_KEYUSED:
        // ������� �����, ������� ����� �� ������ ���� ������������. �������� �� ������.
            printf("Key used. failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_MEMERROR:
        // ���� �������� ������ ���������� � �������� ���������� ������, ��� ������.
            printf("Memory error. failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}

/*
 * ������� ������� ��� ������ � ����� ������.
 * ��������� ������� addEntry �� ������������� ���������� H_KEYUSED.
*/
int test_sameKey()
{
    // ������ �������.
    int random_number = rand() % MAX_RND;
    printf("The number is %d\n", random_number);
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);
    
    // ������� �������.
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // ������� ������ ������.
    if (addEntry( book, random_key, (void*)random_number) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("First entry added\n");
    
    // ������� ������ � ��� �� ������.
    int result = addEntry( book, random_key, (void*)(random_number+5));
    
    switch (result)
    {
        case H_SUCCESS:
        // ������ � ������������ ������ �������. ������.
            printf("New entry with same hash was added. Failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_KEYUSED:
        // ��������, ��� ���� �����������. ���������� ���������.
            printf("Key used. Success\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // ������ ������.
            printf("Memory error. Failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}   
   
/*
 * ������� ������� ������ � ���-�������, ���������� �����, � ����� ������� ��� �����.
 * ��������� ������������ ������ ������� addEntry.
 * ��������� ������� getEntry �� ������������� ���������� H_SUCCESS.
*/
int test_getInt(void)
{
    // ���������� �������
    int random_number = rand() % MAX_RND;
    printf("The number is %d\n", random_number);
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);
    void* address;
    
    // ������� �������
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // ������� ������.
    if (addEntry( book, random_key, (void*)random_number) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Entry was added\n");
    
    // �������� ������� ������
    int result = getEntry( book, random_key, &address);
     
    switch (result)
    {
        case H_SUCCESS:
            if (random_number == (int)address)
            {
            // ���� ��������� ������ ������������� ��������, �������� �� ������.
                printf("Success, returned %d.\n", (int)address);
                deleteTable(book);
                free(random_key);
                return TEST_SUCCESS;
            }
            else
            {
            // ���� ������ ����������, �������� � �������.
                printf("Data damaged, now it's %d\n", (int)address);
                deleteTable(book);
                free(random_key);
                return TEST_FAIL;
            }
        case H_NOKEY:
        // ������ ��� ����������� ������ ���������� ������� � �������� �� ������.
            printf("Added entry was not founded\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_MEMERROR:
        // ������ ������.
            printf("Memory error � failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}

/*
 * ������� �������� �������������� ������.
 * ��������� ������� getEntry �� ��������� H_NOKEY.
*/
int test_noGet(void)
{
    // ���������� �������
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);
    void* address;
    
    // ������� �������
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
     
    // �������� ������� ������
    int result = getEntry( book, random_key, &address);
     
    switch (result)
    {
        case H_SUCCESS:
        // ���� ���-�� ���������, �� �� �����.
            printf("Failed, returned %d.\n", (int)address);
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_NOKEY:
        // ������ �� �������, ��� � ���������.
            printf("Nothing was found.\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // ������ ������.
            printf("Memory error � failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;
    }
}

/*
 * ������� ������� ������ � ���-�������, ���������� ��������� � ������� �����-���� ���� �� ��.
 * ��������� ������������ ������ ������� addEntry.
 * ��������� ������� getEntry �� ������������� ���������� H_SUCCESS.
*/
int test_getStructField(void)
{
    // ���������� �������
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key and the field name is '%s'\n", random_key);

    typedef struct 
    {
        char name[4];
        char city[4];
        int phone;
    } record;
    record value;
    strcpy(value.city, random_key);
    
    record* address;    
    
    // ������� �������
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // ������� ������.
    if (addEntry( book, random_key, (void*)&value) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Entry was added\n");
    
    // �������� ������� ������
    int result = getEntry( book, random_key, (void**)&address);
     
    switch (result)
    {
        case H_SUCCESS:
            if (!strcmp(address->city, random_key))
            {
            // ���� ��������� ������ ������������� ��������, �������� �� ������.
                printf("Success, returned '%s'.\n", address->city);
                deleteTable(book);
                free(random_key);
                return TEST_SUCCESS;
            }
            else
            {
            // ���� ������ ����������, �������� � �������.
                printf("Data damaged, now it's %d\n", (int)address);
                deleteTable(book);
                free(random_key);              
                return TEST_FAIL;
            }
        case H_NOKEY:
        // ������ ��� ����������� ������ ���������� ������� � �������� �� ������.
            printf("Added entry was not founded\n");
            deleteTable(book);
            free(random_key);          
            return TEST_FAIL;
        case H_MEMERROR:
        // ������ ������.
            printf("Memory error � failed\n");
            deleteTable(book);
            free(random_key);     
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}

/*
 * ������� ��������� ��������� ������.
 * ��������� ������������ ������ ������� removeEntry �� ��������� H_NOKEY.
*/
int test_remove()
{
    // ������ �������
    int random_number = rand() % MAX_RND;
    printf("The number is %d\n", random_number);
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);
    void* address;
    
    // ������� �������
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // ��������� ������
    if (addEntry( book, random_key, (void*)random_number) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        return TEST_ERROR;
    }
    printf("Entry was added\n");
    
    // ������� ������
    switch (removeEntry( book, random_key))
    {
        case H_NOKEY:
        // ������ ��� ����������� ������ ����������: ������.
            printf("Added entry was not founded\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_MEMERROR:
            printf("Memory error. Failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_SUCCESS:
        // ������ ������� �������, ����� ����������.
            printf("Entry was removed\n");
            break;
    }   
    
    // �������� �������� ��������� ������
    int result = getEntry( book, random_key, &address);
     
    switch (result)
    {
        case H_SUCCESS:
        // ������ ���������, � �� ����� ��� ��� ������ ���� ������. ������.
            printf("Deleted entry was found, returned %d. Failed\n", (int)address);
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_NOKEY:
        // ������ �� ���������, �.�. ���� �������. �����.
            printf("Deleted entry was not founded. Success.\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // ������ ������.
            printf("Memory error � failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;    
    }
}

/*
 * ������� ������� �������������� ������.
 * ��������� ������������� ���������� H_SUCCESS ������� removeEntry.
*/
int test_noRemove(void)
{
    // ���������� �������
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);

    // ������� �������
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
     
    // �������� ������� ������
    int result = removeEntry( book, random_key);
     
    switch (result)
    {
        case H_SUCCESS:
        // ���� ���-�� ���������, �� �� �����.
            printf("Failed, returned.\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_NOKEY:
        // ������ �� �������, ��� � ���������.
            printf("Nothing was found.\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // ������ ������.
            printf("Memory error � failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // ����������� ������
            printf("Unknown error\n");
            return TEST_FAIL;
    }
}

/*
 * �������� ����: ��������� ���-������� ����� ������� ����������� ������� � ����� ��������� ������ ������.
 * ����������� ������ ���� ������� � ������, ����� ������ ���������� ������.
*/
int test_fullLoad()
{
    // ������� �����.
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        deleteTable(book);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
    
    int tsize =5000;
    printf("%d entries will be added\n", tsize);
    
    // ��������� ����������
    void* address;
    int i;
    char* key = malloc(sizeof(char)*KEY_MAXLEN);
   
    // ������� ��������� ������� ����������
    for (i = 0; i < tsize; i++)
    {
        // ������� ����� ���� � ���������� �� ���� ������� �����.
        snprintf(key, KEY_MAXLEN, "hash%d", i);
        if (addEntry( book, key, (void*)i) != H_SUCCESS)
        {
        // ���� ������ ���� �� ��� ����� ����������, ���� �������.
            printf("Couldn't add entry #%d\n", i);
            free(key);
            deleteTable(book);
            return TEST_FAIL;
        }
    }
    printf("All entries were added\n");
    
    // ������� ��������� ������� ���������� ��� ���
    for (i = 0; i < tsize; i++)
    {
        // ������� ����� ���� � ���������� �� ���� ������� �����.
        snprintf(key, KEY_MAXLEN, "hash%d", i);
        if (addEntry( book, key, (void*)i) != H_KEYUSED)
        {
        // ���� ������ ���� �� ��� ����� ����������, ���� �������.
            printf("Entry #%d was created twice.\n", i);
            free(key);
            deleteTable(book);
            return TEST_FAIL;
        }
    }
    printf("All entries weren't added twice.\n");    
        
    for (i = 0; i < tsize; i++)
    {
        // ��������� ��� ��������.
        snprintf(key, KEY_MAXLEN, "hash%d", i);    
        if (getEntry( book, key, &address) != H_SUCCESS)
        {
            printf("Couldn't get entry #%d\n", i);
            deleteTable(book);
            free(key);
            return TEST_FAIL;
        }
        if ((int)address != i)
        {
            printf("Data in entry #%d damaged", i);
            deleteTable(book);
            free(key);
            return TEST_FAIL;            
        }
    }
    printf("All entries were readed\n");
    
    // ��������� ����� � ����������� ������� ������� � �������������� ������. 
    key = randKey(); // ���� ���� �������� �� �������� � ���������������, �.�. � ��� ��� ����.
    if (getEntry( book, key, &address) != H_NOKEY)
    {
        printf("Uncreated entry was read\n");
        deleteTable(book);
        free(key);
        return TEST_FAIL;
    }
    printf("Uncreated entry wasn't read\n");
    
    // ������� ��� ��������.
    for (i = 0; i < tsize; i++)
    {
        snprintf(key, KEY_MAXLEN, "hash%d", i);    
        if (removeEntry( book, key) != H_SUCCESS)
        {
            printf("Couldn't remove #%d\n", i);
            deleteTable(book);
            free(key);
            return TEST_FAIL;
        }
    }
    printf("All entries were removed\n"); 
    
    // ���������, ��� �������� ���������.
    for (i = 0; i < tsize; i++)
    {
        snprintf(key, KEY_MAXLEN, "hash%d", i);    
        if (removeEntry( book, key) != H_NOKEY)
        {
            printf("Entry #%d was removed twice. Test failed.\n", i);
            deleteTable(book);
            free(key);
            return TEST_FAIL;
        }
    }
    printf("All entries weren't removed twice.\n"); 

    deleteTable(book);
    free(key);
    
    return TEST_SUCCESS;
}

/*
 * ������ ���� ������
 * ��������� ����������
 * ����� ������� ���� �� ���� ����, ��������� ���������� -1
*/
int main()
{
    int rating = 0;
    srand(time(NULL)); // ����������� ���������� ��������� �����.
    printf("\n");    
    // ���� ���� ���������� � �������, �������� rating ������������� �� �������.
    rating += test(&test_add, "Adding int-element");
    rating += test(&test_sameKey, "Non-adding 2 elements with same key");
    rating += test(&test_getInt, "Getting int-element");
    rating += test(&test_noGet, "Non-getting element from empty table");
    rating += test(&test_getStructField, "Getting struct field");
    rating += test(&test_remove, "Removing int-element");
    rating += test(&test_noRemove, "Non-removing element from empty table");
    rating += test(&test_fullLoad, "Full-loading hash-table");
    if (rating)
    {
        printf("\n>> %d tests were failed\n", rating);
        return -1;
    }
    else
    {
        printf("\n>> All tests were succeeded\n");
        return 0;
    }
}