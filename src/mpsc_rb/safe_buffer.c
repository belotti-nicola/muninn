#include <internal/safe_buffer.h>

#include <string.h>

bool sb_init(safe_buffer_t *sb, uint8_t *buffer, size_t buffer_size)
{
    if ( sb == NULL || buffer == NULL || buffer_size == 0)
    {
        return false;
    }

    sb->buffer = buffer;
    sb->buffer_size = buffer_size;

    memset(buffer,0,buffer_size);

    return true;
}

bool sb_copy_out(const safe_buffer_t *sb, size_t offset, uint8_t *dest, size_t len)
{
    if ( sb == NULL || dest == NULL || len == 0) return false;

    size_t size = sb->buffer_size;
    
    if ( size < len) return false;

    if ( offset > size - 1) return false;

    uint8_t *src = sb->buffer + offset;
    size_t bytes_copied = 0;
    if ( offset + len > size - 1)
    {
        bytes_copied = size - offset;
        memcpy(dest,src,bytes_copied);
        dest += bytes_copied;
        src   = sb->buffer;
    }

    memcpy(dest,src,len - bytes_copied);

    return true;
}

bool sb_copy_in(safe_buffer_t *sb, size_t offset, const uint8_t *source, size_t len)
{
    if ( sb == NULL || source == NULL || len == 0 ) return false;

    size_t size = sb->buffer_size;

    if ( len > size ) return false;

    if ( offset > size - 1) return false;

    uint8_t *ptr = sb->buffer + offset;
    size_t bytes_copied = 0;
    if ( offset + len > size - 1)
    {
        bytes_copied = size - offset;
        memcpy(ptr,source,bytes_copied);
        source +=  bytes_copied;
        ptr = sb->buffer;
    }
    memcpy(ptr,source,len - bytes_copied);

    return true;

}