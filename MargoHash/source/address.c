/*
 * address.c
 * Реализация записной книги с использованием хэш-таблицы.
 * 
 * Крюков Павел, 2011 год.
*/

#include <stdio.h>

#include "hash.h"

int main()
{
    TABLE* address = createTable();
    char command[10];
    char surname[20];
    int number;
    int work = 1;
    while (work)
    {
        printf("\nEnter command: ADD, SEARCH, REMOVE, EXIT\n");
        scanf("%s", command);
        if (!strcmp(command, "ADD"))
        {
            printf("Enter surname\n");
            scanf("%s", surname);
            printf("Enter number\n");
            scanf("%d", &number);
            switch (addEntry( address, surname, (void*)number))
            {
                case H_KEYUSED:
                    printf("There is a person with the same surname.\n");
                    break;
                case H_SUCCESS:
                    printf("Person was added.\n");
                    break;
                case H_MEMERROR:
                    printf("Memory error.\n");
                    return -1;
            }
        }
        else
            if (!strcmp(command, "REMOVE"))
            {
                printf("Enter surname\n");
                scanf("%s", surname);
                switch (removeEntry( address, surname))
                {
                    case H_NOKEY:
                        printf("There is no person with the same surname.\n");
                        break;
                    case H_SUCCESS:
                        printf("Person was removed.\n");
                        break;
                    case H_MEMERROR:
                        printf("Memory error.\n");
                        return -1;
                }
            }
            else
                if (!strcmp(command, "SEARCH"))
                {
                    printf("Enter surname\n");
                    scanf("%s", surname);
                    void* adr;
                    switch (getEntry( address, surname, &adr))
                    {
                        case H_NOKEY:
                            printf("There is no person with the same surname.\n");
                            break;
                        case H_SUCCESS:
                            printf("Number is %d\n", (int)adr);
                            break;
                        case H_MEMERROR:
                            printf("Memory error.\n");
                            return -1;
                    }
                }
                else
                    if (!strcmp(command, "EXIT"))
                    {
                        work = 0;
                    }
    }
    deleteTable( address);
    return 0;
}