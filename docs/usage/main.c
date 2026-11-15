#include <muninn.h>

#include <unistd.h>

#define POC_LOG "poc.log"

int main()
{
  remove(POC_LOG);

  CONFIG CONF;
  muninn_config_default(&CONF);

  muninn_message_mask mask = MEDM_SEVERITY | MEDM_MESSAGE;
  muninn_config_set_mask(&CONF,mask);

  muninn_config_set_file(&CONF,true,POC_LOG);
  muninn_config_set_console(&CONF,true,true);


  muninn_t m;
  muninn_init(&m,&CONF);
  
  muninn_log_debug(&m,"This is a debug log");
  muninn_log_info(&m,"This is a info log");
  muninn_log_warning(&m,"This is a warning log");
  muninn_log_error(&m,"This is a error log");
  muninn_log_fatal(&m,"This is a fatal log");

  muninn_shutdown(&m);
  return 0;
}
