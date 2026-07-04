#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <muninn.h>
#include <string.h>
#include <dirent.h>


#include "test_utils.h"

#define PATHSIZE 200

#define NUM_THREADS        20
#define MSG_PER_THREAD   5000

#define MESSAGE_SIZE      300

void* stress_producer_routine(void *arg)
{
    muninn_t *m = (muninn_t *)arg;
    char msg_buffer[MESSAGE_SIZE];
    memset(msg_buffer,(int)'a',MESSAGE_SIZE);
    msg_buffer[MESSAGE_SIZE-1] ='\0';

    for (int i = 0; i < MSG_PER_THREAD; i++)
    {
        muninn_log_info(m,msg_buffer);
    }

    return NULL;
}

long countRows(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0; // Se un file non si apre, restituiamo 0 per non bloccare il conteggio degli altri
    }

    char buffer[65536]; // Buffer generoso da 64KB
    long rows = 0;
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                rows++;
            }
        }
    }

    fclose(fp);
    return rows;
}

long countTotalRowsWithPrefix(const char *dir_path, const char *prefix) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Errore nell'apertura della directory di test");
        return -1;
    }

    struct dirent *entry;
    long total_rows = 0;
    size_t prefix_len = strlen(prefix);

    printf("\n--- Scansione file di log generati ---\n");
    while ((entry = readdir(dir)) != NULL) {
        // Controlliamo se il nome del file inizia con il nostro prefisso (es. "test_stressing")
        if (strncmp(entry->d_name, prefix, prefix_len) == 0) {
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
            
            long file_rows = countRows(full_path);
            total_rows += file_rows;
            
            printf("Trovato: %s -> %ld righe\n", entry->d_name, file_rows);
        }
    }

    closedir(dir);
    return total_rows;
}

int main(void)
{
    char testlog[PATHSIZE];
    if(compute_test_file_name(testlog,PATHSIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file.");
    }

    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,testlog);
    muninn_config_set_console(&CONF,false,false);
    muninn_config_set_compressor(&CONF,false);

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    pthread_t competitors[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&competitors[i], NULL, stress_producer_routine, &muninn);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(competitors[i],NULL);
    }

    muninn_shutdown(&muninn);

    char cwd[PATHSIZE];
    if(get_executable_directory(cwd,PATHSIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: could not detect executable directory.");
        return 1;
    }
    if(countTotalRowsWithPrefix(cwd,"test_stressing") < MSG_PER_THREAD * NUM_THREADS * 0.99 )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("countRows fail:%ld instead of %f.",countTotalRowsWithPrefix(cwd,"test_stressing"),NUM_THREADS * MSG_PER_THREAD * 0.99);
        return 1;
    }
    

    return 0;
}