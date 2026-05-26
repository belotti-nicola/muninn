#include <muninn.h>

int main()
{
  muninn_t m;
  muninn_init("poc_muninn.log",&m);
  muninn_log_info(&m,"Hello");
  muninn_log_info(&m," ");
  muninn_log_info(&m,"World");

  muninn_shutdown(&m);
  return 0;
}
