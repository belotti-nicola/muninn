#include <internal/muninn_config.h>

#include <string.h>

void muninn_config_default(CONFIG *cfg) 
{
    if (!cfg) return;
    memset(cfg, 0, sizeof(CONFIG));
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