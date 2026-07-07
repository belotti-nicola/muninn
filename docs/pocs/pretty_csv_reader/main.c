#include "csv_reader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define CSV "codec_dataset.csv"

typedef enum ENs
{
    EN1,
    EN2,
    EN3,
    EN4

} ENs;

typedef struct CSV_DATA
{
    ENs en;
    int a,b,c,d;

} CSV_DATA;

long int format_int(char *ptr)
{
    char *endptr;
    long int value = strtol(ptr,&endptr,16);

    if (*endptr == '\0') 
    {
        return value;
    }

    //fallback
    value = strtol(ptr,&endptr,0);
    if (*endptr == '\0') 
    {
        return value;
    }

    return 0;
}

int main()
{
    CSV_DATA data;


    FILE *file = fopen(CSV,"r");
    if(file == NULL) 
    {
        printf("Error!%d",__LINE__);
        return 1;
    }

    char ch;
    char line_buffer[2048];
    size_t idx = 0;

    while ((ch = fgetc(file)) != EOF) 
    {
        if (ch == '\n' || ch == '\r' || ch == '\t') continue;

        line_buffer[idx++] = ch;

        if (ch == ';') 
        {
            line_buffer[idx] = '\0'; 
            
            char *token = strtok(line_buffer, ",;");
            if (token != NULL) 
            {
                if(strcmp(token,"EN1") == 0)
                {
                    data.en = EN1;
                }
                if(strcmp(token,"EN2") == 0)
                {
                    data.en = EN2;
                }
                if(strcmp(token,"EN3") == 0)
                {
                    data.en = EN3;
                }
                if(strcmp(token,"EN4") == 0)
                {
                    data.en = EN4;
                }
            }
            
            token = strtok(NULL, ",;");
            if (token != NULL) 
            {
                char *endptr;
                unsigned long value = strtoul(token,&endptr,16);

                if (*endptr != '\0') 
                {
                    printf("Error %d\n",__LINE__);
                    return 1;
                }
                data.a = (int)value;
            }

            token = strtok(NULL, ",;");
            if (token != NULL) 
            {
                char *endptr;
                unsigned long value = strtoul(token,&endptr,10);

                if (*endptr != '\0') 
                {
                    printf("Error %d\n",__LINE__);
                    return 1;
                }
                data.b = (int)value;
            }

            token = strtok(NULL, ",;");
            if (token != NULL) 
            {
                
                data.c = (int)format_int(token);
            }

            token = strtok(NULL, ",;");
            if (token != NULL) 
            {
                
                data.d = (int)format_int(token);
            }
            
            
            idx = 0; 
        }
    }

    printf("End en %u\n",data.en);
    printf("End en %u\n",data.a);
    printf("End en %u\n",data.b);
    printf("End en %u\n",data.c);
    printf("End en %u\n",data.d);

    return 0;
}