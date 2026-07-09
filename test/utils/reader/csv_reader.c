#include "csv_reader.h"

bool csvreader_open(CSVReader *reader, const char *path)
{
    if(reader == NULL || reader->records == NULL) return false;

    FILE *ptr = fopen(path,"r");
    if(ptr == NULL) return false;

    reader->file         = ptr;
    reader->records_size = 0;

    return true;
}

bool csvreader_next(CSVReader *reader)
{
    if(reader == NULL || reader->file == NULL) return false;

    reader->records_size = 0;

    size_t counter = 0;
    char c;
    while(true)
    {
        if(counter == BUFFER_SIZE) break;

        c = fgetc(reader->file);
        if(c == EOF)
        {
            break;
        }

        if(c == '\r' || c == '\n' || c == '\t') continue;
        
        if(c != ';')
        {
            reader->buffer[counter] = c;
            reader->buffer[counter+1] = '\0';
            counter++;
            continue;
        }

        break;
    }

    if(counter == 0) return false;

    bool started = false;
    for(size_t tmp=0 ; tmp < counter ; tmp++ )
    {
        char c = reader->buffer[tmp];
       
        if(c == ',')
        {
            size_t offset       = reader->records_size;
            reader->buffer[tmp] = '\0';
            started  = false;
            continue;
        }

        if(started == false)
        {
            size_t offset = reader->records_size;
            reader->records[offset] = reader->buffer + tmp;
            reader->records_size += 1;
            started = true;
            continue;
        }
    }

    return true;
}

bool csvreader_close(CSVReader *reader)
{
    if(reader == NULL || reader->file == NULL) return false;

    return true;
}