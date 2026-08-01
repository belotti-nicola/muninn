#ifndef MCONFIG_H
#define MCONFIG_H

#include <stdbool.h>
#include <internal/muninn_int.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>


typedef struct {
    bool enabled;
    char path[P_SIZE];
} file_logger_cfg_t;

typedef struct {
    bool enabled;
    bool colored;
} console_logger_cfg_t;

typedef struct {
    bool enabled;
} compressor_cfg_t;

typedef struct CONFIG 
{
    muninn_message_mask  mask;

    file_logger_cfg_t    file;
    console_logger_cfg_t console;
    compressor_cfg_t     compressor;
} CONFIG;


void muninn_config_default(CONFIG *cfg);
void muninn_config_set_file(CONFIG *cfg, bool enabled, const char *path);
void muninn_config_set_console(CONFIG *cfg, bool enabled, bool colored);
void muninn_config_set_compressor(CONFIG *cfg, bool enabled);
void muninn_config_set_mask(CONFIG *cfg, muninn_message_mask mask);


#endif