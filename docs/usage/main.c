#include <muninn.h>

int main()
{
  muninn_t m;
  muninn_init("poc_muninn.log",&m);
  muninn_log(&m,"Hello");
  muninn_log(&m," ");
  muninn_log(&m,"World");

  muninn_shutdown(&m);
  return 0;
}
