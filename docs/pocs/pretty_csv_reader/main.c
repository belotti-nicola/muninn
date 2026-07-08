#include "csv_reader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define CSV "codec_dataset.csv"

#define TRACESTR(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

int main()
{
    CSVReader reader = {0};

    bool rc;
    rc = csvreader_open(&reader,CSV);
    if(rc == false)
    {
        TRACESTR("Error: open returned false");
        return 1;
    }

    while(csvreader_next(&reader))
    {
        TRACESTR("Record found(%ld fields).",reader.records_size);
        for(int i=0;i<reader.records_size;i++)
        {
            TRACESTR("%d:",i);
            TRACESTR("\t%s",reader.records[i]);
        }
    }

    TRACESTR("End.");
    return 0;
}