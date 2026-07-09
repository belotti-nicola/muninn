#include "csv_reader.h"
#include <stdio.h>

bool csv_init(CSVReader *reader, const char *filename, char delimiter,int max_fields)
{
    if(reader == NULL || filename == NULL) return false;

    FILE *ptr = fopen(filename,"r");
    if(ptr == NULL)
    {
        fprintf(stderr,"Error opening %s",filename);
        return false;
    }

    reader->delimiter     = delimiter;
    reader->fields_number = 0;
    reader->max_fields    = max_fields;

    return true;
}

bool csv_next_record(CSVReader *reader)
{
    if (fgets(reader->line, sizeof(reader->line), reader->file) == NULL) 
    {
        return false;
    }

    reader->line[strcspn(reader->line, "\r\n")] = '\0';

    int i = 0;
    char *token = strtok(reader->line, reader->delimiter);
    
    while (token != NULL && i < reader->max_fields) 
    {
        reader->curr[i] = token;
        i++;
        token = strtok(NULL, reader->delimiter);
    }
    
    reader->fields_number = i;
    return true;
}
void csv_close(CSVReader *reader)
{
    FILE *ptr = reader->file;
    if(ptr != NULL)
    {
        fclose(ptr);
    }
}