/*
 * @Author: gongluck 
 * @Date: 2020-11-18 21:27:47 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-18 21:41:36
 */

// gcc *.c -lm

#include <stdio.h>
#include "cJSON.h"

int main()
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddBoolToObject(root, "bool", cJSON_False);
    cJSON_AddStringToObject(root, "str", "cJSON");

    char* str = cJSON_Print(root);
    printf("%s\n", str);
    cJSON_Delete(root);
    root = NULL;

    root = cJSON_Parse(str);
    cJSON* b = cJSON_GetObjectItem(root, "bool");
    if(b->type == cJSON_True || b->type == cJSON_False)
        printf("bool value : %d\n", b->type);
    cJSON* s = cJSON_GetObjectItem(root, "str");
    if(s->type == cJSON_String)
        printf("str value : %s\n", s->valuestring);
    cJSON_Delete(root);
    return 0;
}