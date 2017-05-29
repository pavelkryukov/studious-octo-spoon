/* 
 * test3.c
 * 
 * Набор тестов универсальной хэш-таблицы.
 *
 * (C) 2011 Крюков Павел, гр. 913.
*/

#include <stdio.h>     // printf, snprintf
#include <stdlib.h>    // rand, srand
#include <string.h>    // strcpy, strcmp
#include <time.h>      // time

#include "hash.h"     // реализация хэш-таблицы

#define TEST_SUCCESS 0  // успешное завершение теста
#define TEST_FAIL 1     // тест не пройден (ошибка в реализации)
#define TEST_ERROR 2    // во время теста произошла ошибка

#define KEY_LEN 4      // длина случайного ключа, используемого для теста
#define KEY_LETTERS 20 // количество букв ASCII, используемых для создания случайного ключа
#define MAX_RND 64     // максимально возможное случайное число
#define KEY_MAXLEN 20  // максимально возможная длина сгенерированного из индекса ключа

/*
 * Интерфейс теста
 * Принимает адрес функции, выполняющий тест и название теста.
 * При удачном выполнении теста возвращает 0, иначе — 1.
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
 * Создание случайного ключа длины KEY_LEN
 * Если память не выделилась, то возвращается NULL.
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
 * Тесты не принимают никак аргументов.
 * Возвращаемые значения: 
 *    TEST_SUCCESS 0  — успешное завершение теста
 *    TEST_FAIL 1     — тест не пройден (ошибка в реализации)
 *    TEST_ERROR 2    — во время теста произошла ошибка
*/

/*
 * Пробуем создать запись в хэш-таблице, содержащую число.
 * Проверяем выделение памяти в функции addEntry.
*/
int test_add(void)
{
    // Определяем условия
    int random_number = rand() % MAX_RND;
    printf("The number is %d\n", random_number);
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }
    printf("The key is '%s'\n", random_key);
    
    // Пытаемся создать таблицу. Если что-то происходит неверно, выходим.
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
    
    // Проводим попытку добавления записи.
    int result = addEntry( book, random_key, (void*)random_number); 
     
    switch (result)
    {
        case H_SUCCESS:
        // Если запись добавилась, сообщаем об успехе
            printf("Success\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_KEYUSED:
        // Таблица пуста, поэтому ключи не должны быть использованы. Сообщаем об ошибке.
            printf("Key used. failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_MEMERROR:
        // Если создание записи происходит с неверным выделением памяти, это ошибка.
            printf("Memory error. failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}

/*
 * Пробуем создать две записи с одним ключом.
 * Проверяем функцию addEntry на достоверность результата H_KEYUSED.
*/
int test_sameKey()
{
    // Задаем условия.
    int random_number = rand() % MAX_RND;
    printf("The number is %d\n", random_number);
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);
    
    // Создаем таблицу.
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // Создаем первую запись.
    if (addEntry( book, random_key, (void*)random_number) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("First entry added\n");
    
    // Создаем запись с тем же ключом.
    int result = addEntry( book, random_key, (void*)(random_number+5));
    
    switch (result)
    {
        case H_SUCCESS:
        // Запись с существующим ключом создана. Ошибка.
            printf("New entry with same hash was added. Failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_KEYUSED:
        // Сообщено, что ключ использован. Правильное поведение.
            printf("Key used. Success\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // Ошибка памяти.
            printf("Memory error. Failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}   
   
/*
 * Пробуем создать запись в хэш-таблице, содержащую число, и потом вернуть это число.
 * Проверяем правильность работы функции addEntry.
 * Проверяем функцию getEntry на достоверность результата H_SUCCESS.
*/
int test_getInt(void)
{
    // Определяем условия
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
    
    // Создаем таблицу
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // Создаем запись.
    if (addEntry( book, random_key, (void*)random_number) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Entry was added\n");
    
    // Пытаемся считать запись
    int result = getEntry( book, random_key, &address);
     
    switch (result)
    {
        case H_SUCCESS:
            if (random_number == (int)address)
            {
            // Если считанные данные соответствуют исходным, сообщаем об успехе.
                printf("Success, returned %d.\n", (int)address);
                deleteTable(book);
                free(random_key);
                return TEST_SUCCESS;
            }
            else
            {
            // Если данные повреждены, сообщаем о неудаче.
                printf("Data damaged, now it's %d\n", (int)address);
                deleteTable(book);
                free(random_key);
                return TEST_FAIL;
            }
        case H_NOKEY:
        // Только что добавленная запись неожиданно исчезла — сообщаем об ошибке.
            printf("Added entry was not founded\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_MEMERROR:
        // Ошибка памяти.
            printf("Memory error — failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}

/*
 * Пробуем получить несуществующую запись.
 * Проверяем функцию getEntry на результат H_NOKEY.
*/
int test_noGet(void)
{
    // Определяем условия
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);
    void* address;
    
    // Создаем таблицу
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
     
    // Пытаемся считать запись
    int result = getEntry( book, random_key, &address);
     
    switch (result)
    {
        case H_SUCCESS:
        // Если что-то считалось, то всё плохо.
            printf("Failed, returned %d.\n", (int)address);
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_NOKEY:
        // Запись не найдена, как и ожидалось.
            printf("Nothing was found.\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // Ошибка памяти.
            printf("Memory error — failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;
    }
}

/*
 * Пробуем создать запись в хэш-таблице, содержащую структуру и вернуть какое-либо поле из неё.
 * Проверяем правильность работы функции addEntry.
 * Проверяем функцию getEntry на достоверность результата H_SUCCESS.
*/
int test_getStructField(void)
{
    // Определяем условия
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
    
    // Создаем таблицу
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // Создаем запись.
    if (addEntry( book, random_key, (void*)&value) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Entry was added\n");
    
    // Пытаемся считать запись
    int result = getEntry( book, random_key, (void**)&address);
     
    switch (result)
    {
        case H_SUCCESS:
            if (!strcmp(address->city, random_key))
            {
            // Если считанные данные соответствуют исходным, сообщаем об успехе.
                printf("Success, returned '%s'.\n", address->city);
                deleteTable(book);
                free(random_key);
                return TEST_SUCCESS;
            }
            else
            {
            // Если данные повреждены, сообщаем о неудаче.
                printf("Data damaged, now it's %d\n", (int)address);
                deleteTable(book);
                free(random_key);              
                return TEST_FAIL;
            }
        case H_NOKEY:
        // Только что добавленная запись неожиданно исчезла — сообщаем об ошибке.
            printf("Added entry was not founded\n");
            deleteTable(book);
            free(random_key);          
            return TEST_FAIL;
        case H_MEMERROR:
        // Ошибка памяти.
            printf("Memory error — failed\n");
            deleteTable(book);
            free(random_key);     
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;            
    }
}

/*
 * Пробуем прочитать удаленную запись.
 * Проверяем правильность работы функции removeEntry на результат H_NOKEY.
*/
int test_remove()
{
    // Задаем условия
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
    
    // Создаем таблицу
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
 
    // Добавляем запись
    if (addEntry( book, random_key, (void*)random_number) != H_SUCCESS)
    {
        printf("Couldn't add entry\n");
        return TEST_ERROR;
    }
    printf("Entry was added\n");
    
    // Удаляем запись
    switch (removeEntry( book, random_key))
    {
        case H_NOKEY:
        // Только что добавленная запись недоступна: ошибка.
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
        // Запись успешно удалена, можно продолжать.
            printf("Entry was removed\n");
            break;
    }   
    
    // Пытаемся прочесть удаленную запись
    int result = getEntry( book, random_key, &address);
     
    switch (result)
    {
        case H_SUCCESS:
        // Запись прочитана, в то время как она должна быть стерта. Ошибка.
            printf("Deleted entry was found, returned %d. Failed\n", (int)address);
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_NOKEY:
        // Запись не прочитана, т.е. была удалена. Успех.
            printf("Deleted entry was not founded. Success.\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // Ошибка памяти.
            printf("Memory error — failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;    
    }
}

/*
 * Пробуем удалить несуществующую запись.
 * Проверяем достоверность результата H_SUCCESS функции removeEntry.
*/
int test_noRemove(void)
{
    // Определяем условия
    char* random_key = randKey();
    if (!random_key)
    {
        printf("Couldn't allocate memory for key\n");
        return TEST_ERROR;
    }    
    printf("The key is '%s'\n", random_key);

    // Создаем таблицу
    TABLE* book;
    book = createTable();        
    if (!book)
    {
        printf("Couldn't create table\n");
        free(random_key);
        return TEST_ERROR;
    }
    printf("Table created on %d\n", (int)book);
     
    // Пытаемся считать запись
    int result = removeEntry( book, random_key);
     
    switch (result)
    {
        case H_SUCCESS:
        // Если что-то удалилось, то всё плохо.
            printf("Failed, returned.\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        case H_NOKEY:
        // Запись не найдена, как и ожидалось.
            printf("Nothing was found.\n");
            deleteTable(book);
            free(random_key);
            return TEST_SUCCESS;
        case H_MEMERROR:
        // Ошибка памяти.
            printf("Memory error — failed\n");
            deleteTable(book);
            free(random_key);
            return TEST_FAIL;
        default:
        // Неизвестная ошибка
            printf("Unknown error\n");
            return TEST_FAIL;
    }
}

/*
 * Жестокий тест: заполняем хэш-таблицу вдвое большим количеством записей и после проверяем каждую запись.
 * Проверяется работа всех функций в случае, когда открыт двусвязный список.
*/
int test_fullLoad()
{
    // Создаем книгу.
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
    
    // Служебные переменные
    void* address;
    int i;
    char* key = malloc(sizeof(char)*KEY_MAXLEN);
   
    // Пробуем заполнить таблицу элементами
    for (i = 0; i < tsize; i++)
    {
        // Создаем новый ключ и записываем по нему счётчик цикла.
        snprintf(key, KEY_MAXLEN, "hash%d", i);
        if (addEntry( book, key, (void*)i) != H_SUCCESS)
        {
        // Если ошибка хотя бы при одном добавлении, тест завален.
            printf("Couldn't add entry #%d\n", i);
            free(key);
            deleteTable(book);
            return TEST_FAIL;
        }
    }
    printf("All entries were added\n");
    
    // Пробуем заполнить таблицу элементами ещё раз
    for (i = 0; i < tsize; i++)
    {
        // Создаем новый ключ и записываем по нему счётчик цикла.
        snprintf(key, KEY_MAXLEN, "hash%d", i);
        if (addEntry( book, key, (void*)i) != H_KEYUSED)
        {
        // Если ошибка хотя бы при одном добавлении, тест завален.
            printf("Entry #%d was created twice.\n", i);
            free(key);
            deleteTable(book);
            return TEST_FAIL;
        }
    }
    printf("All entries weren't added twice.\n");    
        
    for (i = 0; i < tsize; i++)
    {
        // Считываем все элементы.
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
    
    // Попробуем найти в заполненной таблице элемент с несуществующим ключом. 
    key = randKey(); // этот ключ заведомо не совпадет с использованными, т.к. в нем нет цифр.
    if (getEntry( book, key, &address) != H_NOKEY)
    {
        printf("Uncreated entry was read\n");
        deleteTable(book);
        free(key);
        return TEST_FAIL;
    }
    printf("Uncreated entry wasn't read\n");
    
    // Удаляем все элементы.
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
    
    // Проверяем, что элементы удалились.
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
 * Запуск всех тестов
 * Аргументы отсуствуют
 * Когда завален хотя бы один тест, программа возвращает -1
*/
int main()
{
    int rating = 0;
    srand(time(NULL)); // выставление генератора случайных чисел.
    printf("\n");    
    // Если тест завершился с ошибкой, значение rating увеличивается на единицу.
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