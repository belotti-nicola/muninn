#include <internal/muninn_int.h>
#include <internal/panic_flusher.h>

#include <unistd.h>  // write(), close()
#include <fcntl.h>   // open(), O_CREAT, O_WRONLY
#include <stdint.h>  // muninn types
#include <muninn.h>


void execute_panic_flush(muninn_t *muninn)
{
    int fd = open(MUNINN_DUMP, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd < 0) return; //at least we tryed

    size_t start = muninn->gateway_rb.ring_buffer.start;        // RB start
    size_t   end = muninn->gateway_rb.ring_buffer.end;          // RB end
    size_t   cap = muninn->gateway_rb.ring_buffer.current_size; // Content size
    uint8_t *buf = muninn->gateway_rb.ring_buffer.data;         // Content

    int written = 0;
    if (start < end) 
    {
        if (write(fd, buf + start, end - start) < 0) goto cleanup;   //TYRANNOSAURUS GOTO IS BACK https://xkcd.com/292/
    }
    else if (start > end) 
    {
        if (write(fd, buf + start, cap - start) < 0) goto cleanup;   //TYRANNOSAURUS GOTO IS BACK https://xkcd.com/292/
        if (write(fd, buf, end) < 0) goto cleanup;                   //TYRANNOSAURUS GOTO IS BACK https://xkcd.com/292/
    }

    cleanup:                                                         //TYRANNOSAURUS GOTO IS BACK https://xkcd.com/292/
    close(fd);
}