#include <internal/muninn_config.h>

#include <string.h>

void muninn_config_default(CONFIG *cfg) 
{
    if (!cfg) return;
    memset(cfg, 0, sizeof(CONFIG));

    muninn_encoding_mask_init(&cfg->encoding_mask,MEDM_MESSAGE);

    muninn_config_set_formatter(cfg,"<msg>");
}

void muninn_config_set_file(CONFIG *cfg, bool enabled, const char *path) 
{
    if (!cfg) return;
    cfg->file.enabled = enabled;
    
    if (path) {
        strncpy(cfg->file.path, path, P_SIZE - 1);
        cfg->file.path[P_SIZE - 1] = '\0'; // Tappo di sicurezza obbligatorio
    }
}

void muninn_config_set_console(CONFIG *cfg, bool enabled, bool colored) 
{
    if (!cfg) return;
    cfg->console.enabled = enabled;
    cfg->console.colored = colored;
}

void muninn_config_set_compressor(CONFIG *cfg, bool enabled)
{
    if (!cfg) return;
    cfg->compressor.enabled = enabled;
}

void muninn_config_set_mask(CONFIG *cfg, muninn_message_mask mask)
{
    muninn_encoding_mask_t *mm_mask = &cfg->encoding_mask;
    muninn_encoding_mask_set(mm_mask,mask);
}

void muninn_config_set_formatter(CONFIG *cfg,const char *pattern)
{
    if ( strlen(pattern) > PATTERN_SIZE)
    {
        char *concrete_pattern = "<msg>";
        strcpy(cfg->file.pattern,concrete_pattern);
        strcpy(cfg->console.pattern,concrete_pattern);
        return;
    }

    strcpy(cfg->file.pattern,pattern);
    strcpy(cfg->console.pattern,pattern);
}