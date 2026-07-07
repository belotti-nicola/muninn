#include <internal/protocols/messages/messages_codec.h>
#include <string.h>
#include <inttypes.h>

#include "test_utils.h"

#define BUFFER_SIZE 256


#define U8(x) ((uint8_t)(x))

#define LE16(x) \
    U8((x) & 0xFF), \
    U8(((x) >> 8) & 0xFF)

#define LE32(x) \
    U8((x) & 0xFF), \
    U8(((x) >> 8) & 0xFF), \
    U8(((x) >> 16) & 0xFF), \
    U8(((x) >> 24) & 0xFF)

#define LE64(x) \
    U8(((uint64_t)(x)) & 0xFF), \
    U8((((uint64_t)(x)) >>  8) & 0xFF), \
    U8((((uint64_t)(x)) >> 16) & 0xFF), \
    U8((((uint64_t)(x)) >> 24) & 0xFF), \
    U8((((uint64_t)(x)) >> 32) & 0xFF), \
    U8((((uint64_t)(x)) >> 40) & 0xFF), \
    U8((((uint64_t)(x)) >> 48) & 0xFF), \
    U8((((uint64_t)(x)) >> 56) & 0xFF)

#define STR6(s) \
    (s)[0], (s)[1], (s)[2], (s)[3], (s)[4], (s)[5]
#define STR3(s) \
    (s)[0], (s)[1], (s)[2]
#define STR11(s) \
    (s)[0], (s)[1], (s)[2], (s)[3], (s)[4], (s)[5],(s)[6], (s)[7], (s)[8], (s)[9], (s)[10]

int main()
{
    mnn_data_t mnn;
    char file_buffer[100] = {0};
    char function_buffer[100] = {0};
    char message_buffer[100] = {0};
    mnn.file = file_buffer;
    mnn.func = function_buffer;
    mnn.msg = message_buffer;

    const uint8_t buffer[] = {
    LE32(0xFFFFFFFF),//mask 
    LE32(60094),//pid
    LE64(12345678),//thread id
    LE64(1182859210),//timestamp

    6,//file len
    STR6("main.c"),//file

    LE32(0),//line
    U8(0), //severity

    3,//function len
    STR3("foo"),//function

    11,//message le
    0,
    STR11("Hello World"),//message

    };

    if(mm_decode(&mnn,buffer,53) == false)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Decode generic error!");
        return 1;
    }

    if(mnn.pid != 60094)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Pid %u instead of %u",mnn.pid,60094);
        return 1;
    }

    if(mnn.thread_id != 12345678ULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Thread ID %llu instead of %llu", (unsigned long long)mnn.thread_id, 12345678ULL);
        return 1;
    }
    
    if(mnn.timestamp != 1182859210ULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Timestamp %llu instead of %llu", (unsigned long long)mnn.timestamp, 1182859210ULL);
        return 1;
    }
    
    if(mnn.file_len != strlen("main.c"))
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("File len error %" PRIu8 " of %ld", mnn.file_len,strlen("main.c"));
        return 1;
    }
    if(strncmp(mnn.file,"main.c",mnn.file_len) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error on file content");
        return 1;
    }

    if(mnn.func_len != strlen("foo"))
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("func len error %" PRIu8 " of %ld", mnn.func_len,strlen("foo"));
        return 1;
    }
    if(strncmp(mnn.func,"foo",mnn.func_len) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error on func content");
        return 1;
    }
    
    if(mnn.msg_len != strlen("Hello world"))
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("msg len error %" PRIu16 " of %ld", mnn.msg_len,strlen("Hello World"));
        return 1;
    }
    if(strncmp(mnn.msg,"Hello World",mnn.msg_len) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error on msg content");
        return 1;
    }
    
    return 0;
}