#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "muninn.h"

#define THREAD_COUNT           20
#define MSG_PER_THREAD         50
#define MAX_LINE_SIZE          M_SIZE
#define TEST_LOG_FILE          "stress_test.log"

typedef struct
{
    muninn_t *logger;
    int thread_id;

} worker_arg_t;

static void* worker_fn(void *arg)
{
    worker_arg_t *w = arg;

    char msg[256];
    for (int i = 0; i < MSG_PER_THREAD; ++i)
    {
        snprintf(
            msg,
            sizeof(msg),
            "[THREAD %02d] message %08d",
            w->thread_id,
            i
        );

        muninn_log(w->logger, msg);

        /*
         * Introduce scheduler noise
         * to stress synchronization.
         */
        if ((i % 7) == 0)
        {
            sched_yield();
        }

        if ((i % 101) == 0)
        {
            usleep(10);
        }
    }

    return NULL;
}

static int count_lines(FILE *f)
{
    int lines = 0;
    char buffer[MAX_LINE_SIZE];

    rewind(f);

    while (fgets(buffer, sizeof(buffer), f))
    {
        lines++;
    }

    return lines;
}

static bool line_exists(
    FILE *f,
    const char *expected
)
{
    char buffer[MAX_LINE_SIZE];

    rewind(f);

    while (fgets(buffer, sizeof(buffer), f))
    {
        /*
         * Remove trailing newline
         */
        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strcmp(buffer, expected) == 0)
        {
            return true;
        }
    }

    return false;
}

int main(void)
{
    remove(TEST_LOG_FILE);

    muninn_t logger = {0};
    muninn_init(&logger,TEST_LOG_FILE);
    
    pthread_t threads[THREAD_COUNT];
    worker_arg_t args[THREAD_COUNT];

    /*
     * START MANY PRODUCERS
     */
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        args[i].logger = &logger;
        args[i].thread_id = i;

        int rc = pthread_create(
            &threads[i],
            NULL,
            worker_fn,
            &args[i]
        );

        assert(rc == 0);
    }

    
    /*
     * WAIT ALL PRODUCERS
     */
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    /*
     * SHUTDOWN LOGGER
     */
    muninn_shutdown(&logger);

    /*
     * VERIFY FILE EXISTS
     */
    FILE *f = fopen(TEST_LOG_FILE, "r");

    assert(f != NULL);

    /*
     * VERIFY TOTAL LINE COUNT
     */
    int expected_lines =
        THREAD_COUNT * MSG_PER_THREAD;

    int actual_lines =
        count_lines(f);

    if(actual_lines != expected_lines)
    {
        printf("Error! Actual lines(%d) are different from expected(%d)\n",actual_lines,expected_lines);
        return 1;
    }

    /*
     * VERIFY SOME RANDOM MESSAGES EXIST
     */
    char expected[256];

    snprintf(
        expected,
        sizeof(expected),
        "[THREAD %02d] message %08d",
        0,
        0
    );

    assert(
        line_exists(f, expected)
    );

    snprintf(
        expected,
        sizeof(expected),
        "[THREAD %02d] message %08d",
        THREAD_COUNT - 1,
        MSG_PER_THREAD - 1
    );

    assert(
        line_exists(f, expected)
    );

    snprintf(
        expected,
        sizeof(expected),
        "[THREAD %02d] message %08d",
        THREAD_COUNT / 2,
        MSG_PER_THREAD / 2
    );

    assert(
        line_exists(f, expected)
    );

    /*
     * VERIFY NO CORRUPTED LINES
     */
    rewind(f);

    char line[MAX_LINE_SIZE];

    while (fgets(line, sizeof(line), f))
    {
        /*
         * Every line must end with '\n'
         */
        size_t len = strlen(line);

        assert(len > 0);
        assert(line[len - 1] == '\n');

        /*
         * Basic format validation
         */
        assert(
            strstr(line, "[THREAD ") != NULL
        );

        assert(
            strstr(line, "] message ") != NULL
        );
    }

    fclose(f);

    /*
     * OPTIONAL:
     * run repeated init/shutdown cycles
     * to detect lifecycle bugs.
     */
    for (int i = 0; i < 100; ++i)
    {
        muninn_t tmp;
        muninn_init(&tmp,TEST_LOG_FILE);

        muninn_log(&tmp, "cycle");

        muninn_shutdown(&tmp);
    }

    printf(
        "SUCCESS: logger stress test passed\n"
    );

    return 0;
}