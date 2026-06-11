#define TEST_FILE "/home/nicola/Projects/muninn/test/ring_buffer/full.txt"
#define MAX_TESTS 100
#define MAX_LINE 4096
#define BUFFER_SIZE 1024
#define MAX_POP 100


#include <internal/ring_buffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RB_OPERATIONS(X) \
    X(RB_PUSH)           \
    X(RB_POP)            \
    X(RB_PEEK)

typedef enum {
#define X(name) name,
    RB_OPERATIONS(X)
#undef X
} RB_OPERATION;

static const char *rb_operation_names[] = {
#define X(name) #name,
    RB_OPERATIONS(X)
#undef X
};

RB_OPERATION string_to_op(const char *s)
{
    for (int i = 0; i < sizeof(rb_operation_names)/sizeof(rb_operation_names[0]); i++) {
        if (strcmp(s, rb_operation_names[i]) == 0)
            return (RB_OPERATION)i;
    }

    return -1;
}

typedef struct TestCase
{
    char          buffer[MAX_LINE];

    RB_OPERATION  op;
    size_t        start;
    size_t        end;
    size_t        available_data;
    size_t        current_data_size;
    size_t        pop_bytes;
    size_t        popped_bytes;


} TestCase;

int load_tests(const char *filename, TestCase tests[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { return 0; }

    int count = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp) && count < MAX_TESTS) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "//", 2) == 0)
        continue;//comment

        char *tbuffer = strtok(line, ";");
        if (!tbuffer) continue;
        strcpy(tests[count].buffer, tbuffer);

        char *top = strtok(NULL, ";");
        if (!top) continue;
        tests[count].op = string_to_op(top);

        char *tstart = strtok(NULL, ";");
        if (!tstart) continue;
        tests[count].start = (size_t)atoi(tstart);

        char *tend = strtok(NULL, ";");
        if (!tend) continue;
        tests[count].end = (size_t)atoi(tend);

        char *tavailable_data = strtok(NULL, ";");
        if (!tavailable_data) continue;
        tests[count].available_data = (size_t)atoi(tavailable_data);

        char *tcurrent_data_size = strtok(NULL, ";");
        if (!tcurrent_data_size) continue;
        tests[count].current_data_size = (size_t)atoi(tcurrent_data_size);

        char *tpop_bytes = strtok(NULL, ";");
        if (!tpop_bytes) continue;
        tests[count].pop_bytes = (size_t)atoi(tpop_bytes);

        char *tpopped_bytes = strtok(NULL, ";");
        if (!tpopped_bytes) continue;
        tests[count].popped_bytes = (size_t)atoi(tpopped_bytes);

        count++;
    }

    fclose(fp);

    if(count == 0)
    {
        return -1;
    }
    return count;
}


int main() 
{
    uint8_t     buff[BUFFER_SIZE] = {0};
    ring_buffer rb = {0}; rb_setup(&rb,buff,BUFFER_SIZE);

    TestCase tc[MAX_TESTS];
    int test_number = load_tests(TEST_FILE,tc);
    if(test_number == -1)
    {
        printf("Error at line %d: parse error!",__LINE__);
        return 1;
    }
    if(test_number == 0)
    {
        printf("Error at line %d: there are no tests to execute!",__LINE__);
        return 1;
    }
    for(int i=0;i<test_number;i++)
    {
        RB_OPERATION op = tc[i].op;
        switch(op)
        {
            case RB_PUSH:
            {
                rb_push(&rb,tc[i].buffer,strlen(tc[i].buffer));
                if(rb.start != tc[i].start)
                {
                    printf("Error at line %d at iteration %d: start field computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb.start,tc[i].start);
                    return 1;
                }
                if(rb.end != tc[i].end)
                {
                    printf("Error at line %d at iteration %d: end field computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb.end,tc[i].end);
                    return 1;
                }
                if(rb.current_size != tc[i].current_data_size)
                {
                    printf("Error at line %d at iteration %d: current_size field computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb.current_size,tc[i].current_data_size);
                    return 1;
                }
                if(rb_peek(&rb) != tc[i].available_data)
                {
                    printf("Error at line %d at iteration %d: available_data computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb_peek(&rb),tc[i].available_data);
                    return 1;
                }
                if(strncmp(rb.data + rb.start,tc[i].buffer,rb.end-rb.start-1) != 0)
                {
                    printf("Error at line %d at iteration %d: strncmp fail",__LINE__,i);
                    return 1;
                }
                break;
            }
            case RB_POP:
            {
                uint8_t popped[MAX_POP] = {0};
                size_t popped_dimension = rb_pop(&rb,popped,tc[i].pop_bytes);
                if(popped_dimension != tc[i].popped_bytes)
                {
                    printf("Error at line %d at iteration %d: popped bytes(%ld) differs from expected(%ld)",__LINE__,
                        i,popped_dimension,tc[i].popped_bytes);
                    return 1;
                }
                if(rb.start != tc[i].start)
                {
                    printf("Error at line %d at iteration %d: start field computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb.start,tc[i].start);
                    return 1;
                }
                if(rb.end != tc[i].end)
                {
                    printf("Error at line %d at iteration %d: end field computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb.end,tc[i].end);
                    return 1;
                }
                if(rb.current_size != tc[i].current_data_size)
                {
                    printf("Error at line %d at iteration %d: current_size field computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb.current_size,tc[i].current_data_size);
                    return 1;
                }
                if(rb_peek(&rb) != tc[i].available_data)
                {
                    printf("Error at line %d at iteration %d: available_data computed(%ld) differs from expected(%ld)",__LINE__,
                        i,rb_peek(&rb),tc[i].available_data);
                    return 1;
                }
                if(strncmp(popped,tc[i].buffer,popped_dimension) != 0)
                {
                    printf("Error at line %d at iteration %d: strncmp fail",__LINE__,i);
                    return 1;
                }
                
                break;
            }
            case RB_PEEK:
            {
                break;
            }
            default:
            {
                printf("Error at line %d at iteration %d: unsupported operation %d",__LINE__,i,op);
                return 1;
            }
        }
    }

    return 0;
}