#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* getPOSTVal(char* request, char* key) {
    char* val;
    val = strtok(request, ":=\n");

    while(val != NULL)
    {
        if(strcmp(val, key) == 0) {
            printf("%s", val);
            return strtok(NULL, ":=\n");
        }
        val = strtok(NULL, ":=\n");
    }

    return NULL;
}

char* concat(char* str1, char* str2) {
    if (str1 == NULL) {
        char *str = malloc(strlen(str2) + 1);
        strcpy(str, str2);
        return str;
    }

    if (str2 == NULL) {
        char *str = malloc(strlen(str1) + 1);
        strcpy(str, str1);
        return str;
    }

    char *str = malloc(strlen(str1) + strlen(str2) + 1);
    strcat(str, str1);
    strcat(str, str2);
    return str;
}