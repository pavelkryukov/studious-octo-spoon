/*
 * hash3.c
 *
 * Реализация универсальной хэш-таблицы.
 *
 * (C) 2011 Крюков Павел, гр. 913.
*/

#include <stdlib.h> // malloc, free
#include <string.h> // strcpy, strcmp

#include "hash.h"

#define TABLE_SIZE 269 // размер хэш-таблицы, выбран простым чмслом.

/*
 * Структура, соответствующая ячейке хэш-таблицы
*/
typedef struct ENTRY
{
    char* key; // ключ
    void* data; // указатель на данные
    struct ENTRY* next; // формирование двусвязного списка
    struct ENTRY* prev;
} ENTRY;

/*
 * Хэш-функция.
 * Принимает строку, вычисляет её сумму и приводит к модулю TABLE_SIZE.
 * Возвращает хэш от 0 до TABLE_SIZE-1.
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
 * Конструктор записей.
 * Принимает ключ и данные.
 * Возвращает указатель на запись.
 * Если память невозможно выделить, возвращает NULL.
*/
ENTRY* initEntry( char* key, void* data)
{
    ENTRY* record;      
    record = malloc(sizeof(ENTRY)); // инициализируем место под запись.  
    if (!record)
    {
        return NULL;
    }
    record->key = malloc(sizeof(char)*strlen(key));  
    if (!record->key)
    {
        return NULL;
    }
    strcpy( record->key, key);      // копируем ключ.
    record->data = data;            // копируем информацию.   
    record->next = NULL;            // обнуляем значение следующего элемента.
    record->prev = NULL;
    return record;
}


/*
 * Деструктор записей.
 * Принимает адрес записи.
*/
void destroyEntry( ENTRY* victim)
{
    victim->prev = NULL;
    victim->next = NULL;
    free(victim->key);
    free(victim);
}
    
/*
 * Поиск элемента в списке с заданным ключом
 * Принимает адрес начала списка (where) и ключ.
 * Возвращает адрес искомого элемента или NULL, если элемент не найден.
*/
ENTRY* listSearch( ENTRY* where, char* key)
{
    if (!strcmp(where->key, key))
    {     
    // Если элемент с искомым ключом уже есть в таблице, вернуть его адрес.
        return where;
    }
    else
    {
        if (where->next)
        {
        // Рекурсивно переходим в конец списка, попутно проверяя наличие ключей.
            return listSearch( where->next, key);
        }
        else
        {
        // Если мы дошли до конца, возвращаем NULL.
            return NULL;
        }
    }
}

/*
 * Уничтожение двусвязного списка.
 * Принимает адрес первого элемента списка.
 */
void listDelete(ENTRY* what)
{
    if (what)
    {
    // Если элемент не пустой, то вначале удаляем следующий.
    // Удаления попадут в стек команд и будут выполняться с конца.    
        listDelete(what->next);
        destroyEntry(what);
    }
}

/*
 * Создание таблицы.
 * При неудаче возвращает NULL, иначе — адрес на таблицу.
*/
TABLE* createTable()
{
    ENTRY** book;
    book = malloc(sizeof(ENTRY*) * TABLE_SIZE); // инициализируем массив указателей
    if (book)
    { 
    // Если память выделилась, то заполняем массив нулями и возвращаем в виде указателя на таблицу.
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
 * Добавление записи.
 * Принимает адрес таблицы, ключ и ссылку на данные.
 * В случае успеха возвращает H_SUCCESS.
 * Если произошла ошибка выделения памяти, возвращает H_MEMERROR.
 * Если запись с заданным ключом уже существует, возвращает H_KEYUSED.
*/
int addEntry( TABLE* book, char* key, void* data)
{
    ENTRY** myBook = (ENTRY**)book; // преобразование абстрактного типа к реализованному
    int i = hash(key);  // вычисляем хэш ключа 
    if (!myBook[i])     
    {
    // Если ячейка пуста, попытаемся создать запись в ней.
        ENTRY *record = initEntry( key, data);          
        if (record)
        {
            myBook[i] = record;
            return H_SUCCESS;
        }
        else
        {
        // Если память не выделилась, сообщаем.        
            return H_MEMERROR;
        }
    }
    else
    {     
    // Если ячейка не пуста, проверяем на повторное использование ключа.
        if (listSearch(myBook[i], key))       
        {
        // Если ключ уже использован, сообщаем об этом.
            return H_KEYUSED;
        }
        else
        { 
        // Если ключ не использован, добавляем запись в начало списка.
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
            // Если память не выделилась, сообщаем.
                return H_MEMERROR;
            }
        }
    }
}

/*
 * Поиск записи.
 * Принимает адрес таблицы и ключ.
 * В случае успеха возвращает H_SUCCESS и записывает в *data указатель на данные.
 * Если ключу нет соответствия, возвращает H_NOKEY и записывает в *data NULL
*/
int getEntry( TABLE* book, char* key, void** data)
{
    ENTRY** myBook = (ENTRY**)book; // преобразование абстрактного типа к реализованному.
    int i = hash(key);  // вычисляем хэш ключа. 
    if (!myBook[i]) 
    {
    // Если хэшу ключа в соответствие не поставлена ни одна запись, сообщить о том, что искомому ключу не соответствуют записи.
        *data = NULL;
        return H_NOKEY;
    }
    else
    {
    // Если есть хотя бы одна запись, значит, список не пуст.
    // Производим поиск по списку.
        ENTRY* founded = listSearch( myBook[i], key);
        if (founded)
        {
        // Если что-либо нашлось, отдаем указатель на найденное
            *data = founded->data;
            return H_SUCCESS;
        }
        else
        {
        // Если ничего не нашлось, сообщаем.
            *data = NULL;  
            return H_NOKEY;
        }
    }
}

/*
 * Удаление записи.
 * Принммает ссылку на таблицу и ключ.
 * Если элемент найден и удален, возвращает H_SUCCESS.
 * Если элемент не найден, возвращает H_NOKEY.
 */
int removeEntry( TABLE* book, char* key)
{
    ENTRY** myBook = (ENTRY**)book; // преобразование абстрактного типа к реализованному.
    int i = hash(key);  // вычисляем хэш ключа. 
    if (!myBook[i]) 
    {
    // Если хэшу ключа в соответствие не поставлена ни одна запись, сообщить о том, что искомому ключу не соответствуют записи.
        return H_NOKEY;
    }
    else
    {
    // Если есть хотя бы одна запись, значит, список не пуст.
    // Производим поиск по списку.
        ENTRY* victim = listSearch( myBook[i], key);
        if (victim)
        {
        // Если что-либо нашлось, удаляем элемент.
            if (victim->next)
            {
            // Сообщаем следующему, что сзади него другой элемент.
                victim->next->prev=victim->prev;
            }
            if (victim->prev)
            {
            // Сообщаем предыдущему, что впереди него другой элемент.
                victim->prev->next=victim->next;
            }
            else
            {
            // Если элемент оказался первым, заменяем ссылку в таблице
                myBook[i] = victim->next;
            }
            // Освобождаем память.
            destroyEntry(victim);
            return H_SUCCESS;            
        }
        else
        {
        // Если ничего не нашлось, сообщаем.
            return H_NOKEY;
        }
    }
}

/* 
 * Удаление хэш-таблицы.
 * Принимает адрес таблицы.
 * Если успешно, то возвращет H_SUCCESS.
*/
int deleteTable( TABLE* book)
{
    ENTRY** myBook = (ENTRY**)book; // преобразование абстрактного типа к реализованному.
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