#include <unistd.h>

#define LIB_CMSIS_CORE 0

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pw_log/log.h"
#include "pw_system/target_hooks.h"
#include "pw_thread/detached_thread.h"
#include "pw_thread_freertos/context.h"
#include "pw_thread_freertos/options.h"

namespace {

static constexpr size_t kDisplayDrawThreadStackWords = 512;
static pw::thread::freertos::StaticContextWithStack<
    kDisplayDrawThreadStackWords>
    display_draw_thread_context;

const pw::thread::Options& DisplayDrawThreadOptions() {
  static constexpr auto options =
      pw::thread::freertos::Options()
          .set_name("DisplayDrawThread")
          .set_static_context(display_draw_thread_context)
          .set_priority(static_cast<UBaseType_t>(tskIDLE_PRIORITY + 1));
  return options;
}

void MainTask(void*) {
  const uint LED_PIN = 25;
  const uint WAIT_MS = 1000;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  while (1) {
    gpio_put(LED_PIN, 1);
    sleep_ms(WAIT_MS);
    gpio_put(LED_PIN, 0);
    sleep_ms(WAIT_MS);
  }
}

}  // namespace

namespace pw::system {

void UserAppInit() {
  pw::thread::DetachedThread(DisplayDrawThreadOptions(), MainTask);
}

}  // namespace pw::system
