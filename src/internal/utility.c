#include <internal/utility.h>

void extract_file_name(const char *in, size_t in_size, size_t *offset)
{
    size_t last_index = 0;

    for (int i = 0 ; i < in_size ; i++ )
    {
        char *target = (char *)in + i;
        if ( *target == '/')
        {
            last_index = i;
        }
    }

    if ( last_index == 0)
    {
        *offset = 0;
        return;
    }

    *offset   = last_index + 1;
}