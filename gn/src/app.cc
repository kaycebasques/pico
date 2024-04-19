#include <unistd.h>

#include "pw_log/log.h"

int main() {
  while (true) {
    PW_LOG_INFO("Hello, facades!");
    sleep(5);
  }
  return 0;
}
