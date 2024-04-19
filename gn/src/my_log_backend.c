#include <stdarg.h>
#include <stdio.h>

void pw_log_string_HandleMessageVaList(int level,
                                       unsigned int flags,
                                       const char* module_name,
                                       const char* file_name,
                                       int line_number,
                                       const char* message,
                                       va_list args) {
  printf("\n\nSTART OF CUSTOM LOGGER\n");
  printf("Hello, xyz!\n");
  printf("%d\n", level);
  printf("%u\n", flags);
  printf("%s\n", module_name);
  printf("%s\n", file_name);
  printf("%d\n", line_number);
  printf("%s\n", message);
  if (args) { /* Just suppressing the unused arg error... */
  }
  printf("\nEND OF CUSTOM LOGGER\n\n");
}
