#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>

#define PATH_LEN 100

#define RERUN_TIMES 1
#define TIMES       5000
#define BUFFER_SIZE 200

#include <libgen.h>
#include <unistd.h>

char *get_executable_directory(char *out, size_t out_size)
{
    if (out == NULL || out_size == 0) return NULL;

    char raw_path[PATH_LEN] = {0};

    // 1. Chiediamo al kernel Linux dove si trova l'eseguibile corrente
    // Passiamo sizeof(raw_path) - 1 per lasciare lo spazio per il \0 finale
    ssize_t len = readlink("/proc/self/exe", raw_path, sizeof(raw_path) - 1);
    
    if (len == -1) 
    {
        printf("Error at line %d: could not readlink /proc/self/exe\n", __LINE__);
        return NULL;
    }

    // 2. TRAPPOLA EVITATA: readlink non mette il terminatore, lo mettiamo noi!
    raw_path[len] = '\0'; 
    // Ora raw_path contiene ad esempio: "/home/nicola/Projects/muninn/build/test_logger"

    // 3. Usiamo dirname per isolare la cartella tagliando via il nome del file.
    // 'dir' conterrà: "/home/nicola/Projects/muninn/build"
    char *dir = dirname(raw_path);

    // 4. Copiamo il risultato nel buffer del chiamante in modo sicuro
    int written = snprintf(out, out_size, "%s", dir);
    if (written < 0 || (size_t)written >= out_size)
    {
        printf("Error at line %d: out buffer too small for executable path\n", __LINE__);
        return NULL;
    }

    return out;
}

char *compute_test_file_name(char *out, size_t out_size)
{
    if (out == NULL || out_size == 0) return NULL;

    if (get_executable_directory(out, out_size) == NULL)
    {
        printf("Error at line %d: could not determine directory.\n", __LINE__);
        return NULL;
    }

    size_t cwd_len = strlen(out);
    size_t remaining_space = out_size - cwd_len;

    char *file_basename = basename(__FILE__); // this may include extension

    // find the name of the file 
    char *dot = strrchr(file_basename, '.');
    
    // 3. Calcola la lunghezza del nome SENZA l'estensione.
    // Se c'è un punto, la lunghezza è la distanza tra l'inizio e il punto.
    // Se stranamente non c'è, prendiamo la lunghezza totale.
    int name_len = (dot != NULL) ? (dot - file_basename) : (int)strlen(file_basename);

    // 4. La Magia della snprintf:
    // %.*s significa: "Stampa una stringa, ma prendi la LUNGHEZZA MASSIMA dalla variabile che ti passo prima (name_len)"
    int written = snprintf(out + cwd_len, remaining_space, "/%.*s.log", name_len, file_basename);
    if (written < 0 || (size_t)written >= remaining_space)
    {
        printf("Error at line %d: full path is too long for the buffer.\n", __LINE__);
        return NULL;
    }

    return out;
}

int files_counter(const char* path)
{
    char *f_basename = basename(__FILE__);
    char *dot = strrchr(f_basename, '.');
    int len_before_dot = (dot != NULL) ? (dot - f_basename) : (int)strlen(__FILE__);

    DIR* dir = opendir(path);
    if (!dir) return -1;

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        size_t       len = strlen(name);
        size_t  base_len = strlen(f_basename);

        if(strncmp(name,f_basename,len_before_dot) != 0)
        {
            //prefix is different from __FILE__ without extension
            continue;
        }

        if(len > 4 &&
            strncmp(name+len-4,".lz4",len - 4) == 0)
        {
            count++;
            continue;
        }

        if(len > 9 &&
            strncmp(name+len-9,".rotating",len - 9) == 0)
        {
            count++;
            continue;
        }       
    }

    closedir(dir);
    return count;
}

int main(void)
{
    char filepath[PATH_LEN] = {0};
    if(compute_test_file_name(filepath,PATH_LEN) == NULL)
    {
        printf("Error: could not compute log test file\n");
        return 1;
    }

    muninn_t muninn;
    muninn_init(&muninn,filepath);

    char message[BUFFER_SIZE];
    char a = 'a';
    memset(message,a,BUFFER_SIZE-1);
    message[BUFFER_SIZE-1]='\0';

    for(int i=0;i<RERUN_TIMES;i++)
    {
        // overflow the F_MAX_SIZE limit!
        for(int j=0;j<TIMES;j++)
        {
            muninn_log_info(&muninn,message);
            usleep(50);
        }
    }

    usleep(200);
    muninn_shutdown(&muninn);

    char cwd[PATH_LEN];
    if(get_executable_directory(cwd,PATH_LEN) == NULL)
    {
        printf("Error: could not detect executable directory\n");
        return 1;
    }
    int file_counter = files_counter(cwd);
    if ( file_counter != RERUN_TIMES) 
    {
        printf("Error: lz4 files number differs from expected (found %d files instead of %d).\n",file_counter,RERUN_TIMES);
        return 1;
    }

    return 0;
}