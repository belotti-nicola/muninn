#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024
#define FIELDS_SIZE 800

typedef struct CSVReader
{
    FILE  *file;
    char   buffer[BUFFER_SIZE];
    char  *records[FIELDS_SIZE];
    size_t records_size;

} CSVReader;

bool csvreader_open(CSVReader *reader, const char *path);
bool csvreader_next(CSVReader *reader);
bool csvreader_close(CSVReader *reader);


#endif