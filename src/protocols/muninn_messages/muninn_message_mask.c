#include <internal/protocols/muninn_messages/muninn_message_mask.h>

#include <string.h>
#include <stdio.h>

muninn_message_mask mmm_string_to_edm(char *str)
{
    if (str == NULL || *str == '\0') {
        return MEDM_NONE;
    }

    char local_buf[256];
    strncpy(local_buf, str, sizeof(local_buf) - 1);
    local_buf[sizeof(local_buf) - 1] = '\0';

    muninn_message_mask final_mask = MEDM_NONE;

    char *token = strtok(local_buf, "|");
    while (token != NULL) 
    {
        if      (strcmp(token, "MEDM_ALL") == 0)        final_mask |= MEDM_ALL;
        else if (strcmp(token, "MEDM_FILE") == 0)       final_mask |= MEDM_FILE;
        else if (strcmp(token, "MEDM_THREAD") == 0)     final_mask |= MEDM_THREAD;
        else if (strcmp(token, "MEDM_TIMESTAMP") == 0)  final_mask |= MEDM_TIMESTAMP;
        else if (strcmp(token, "MEDM_SEVERITY") == 0)   final_mask |= MEDM_SEVERITY;
        else if (strcmp(token, "MEDM_MESSAGE") == 0)    final_mask |= MEDM_MESSAGE;
        else if (strcmp(token, "MEDM_LINE") == 0)       final_mask |= MEDM_LINE;
        else if (strcmp(token, "MEDM_FUNCTION") == 0)   final_mask |= MEDM_FUNCTION;
        else if (strcmp(token, "MEDM_PID") == 0)        final_mask |= MEDM_PID;
        else if (strcmp(token, "MEDM_RUNTIME_TS") == 0) final_mask |= MEDM_RUNTIMESTAMP;
        else if (strcmp(token, "MEDM_NONE") == 0)       final_mask |= MEDM_NONE;

        token = strtok(NULL, "|");
    }

    return final_mask;
}

const char *mmm_edm_to_string(muninn_message_mask mask)
{
    if (mask == MEDM_ALL)  return "MEDM_ALL";
    if (mask == MEDM_NONE) return "MEDM_NONE";

    // C11 _Thread_local: Ogni thread ha il suo buffer isolato. Zero Race Conditions.
    static _Thread_local char output_buf[512];
    output_buf[0] = '\0';

    if (mask & MEDM_FILE)         strcat(output_buf, "MEDM_FILE|");
    if (mask & MEDM_THREAD)       strcat(output_buf, "MEDM_THREAD|");
    if (mask & MEDM_TIMESTAMP)    strcat(output_buf, "MEDM_TIMESTAMP|");
    if (mask & MEDM_SEVERITY)     strcat(output_buf, "MEDM_SEVERITY|");
    if (mask & MEDM_MESSAGE)      strcat(output_buf, "MEDM_MESSAGE|");
    if (mask & MEDM_LINE)         strcat(output_buf, "MEDM_LINE|");
    if (mask & MEDM_FUNCTION)     strcat(output_buf, "MEDM_FUNCTION|");
    if (mask & MEDM_PID)          strcat(output_buf, "MEDM_PID|");
    if (mask & MEDM_RUNTIMESTAMP) strcat(output_buf, "MEDM_RUNTIME_TS|");

    size_t len = strlen(output_buf);
    if (len > 0 && output_buf[len - 1] == '|') 
    {
        output_buf[len - 1] = '\0';
    }

    return output_buf;
}