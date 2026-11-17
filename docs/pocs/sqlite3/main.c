#include <sqlite3.h>
#include <stdio.h>

int main()
{
    printf("Versione SQLite attiva: %s\n", sqlite3_libversion());

    return 0;
}