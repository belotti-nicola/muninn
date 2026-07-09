#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FIELDS 50
#define MAX_ROW_LENGHT 1024

typedef struct {
    FILE   *file;
    char   *line;
    char   *curr; 
    int     fields_number;
    char   *delimiter;
    int     max_fields;

} CSVReader;

bool csv_init(CSVReader *reader, const char *filename, char *delimiter,int max_fields);
bool csv_next_record(CSVReader *reader);
void csv_close(CSVReader *reader);

#endif