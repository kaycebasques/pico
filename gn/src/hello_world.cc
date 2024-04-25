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
          // TODO: amontanez - Find a way to better manage priorities.
          .set_priority(static_cast<UBaseType_t>(tskIDLE_PRIORITY + 1));
  return options;
}

void MainTask(void*) {
  // PW_LOG_INFO("Hello, world!");
  const uint LED_PIN = 25;
  const uint WAIT_MS = 1000;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  // gpio_put(LED_PIN, 1);
  while (1) {
    gpio_put(LED_PIN, 1);
    sleep_ms(WAIT_MS);
    gpio_put(LED_PIN, 0);
    sleep_ms(WAIT_MS);
  }
}

}  // namespace

namespace pw::system {

// void UserAppInit() {
//   const uint LED_PIN = 25;
//   gpio_init(LED_PIN);
//   gpio_put(LED_PIN, 1);
//   PW_LOG_INFO("UserAppInit");
//   // pw::thread::DetachedThread(DisplayDrawThreadOptions(), MainTask);
// }

void UserAppInit() {
  pw::thread::DetachedThread(DisplayDrawThreadOptions(), MainTask);
  // const uint LED_PIN = 25;
  // gpio_init(LED_PIN);
  // gpio_set_dir(LED_PIN, GPIO_OUT);
  // gpio_put(LED_PIN, 1);
  // while (1) {
  //   // gpio_put(LED_PIN, 1);
  //   // for (volatile uint32_t i = 0; i < 100000000; i++) {
  //   //   __asm__("nop");
  //   // }
  //   // gpio_put(LED_PIN, 0);
  //   // for (volatile uint32_t i = 0; i < 100000000; i++) {
  //   //   __asm__("nop");
  //   // }
  // }
  // stdio_usb_init();
  // PW_LOG_INFO("pw_system main");
  // pw::system::Init();
  // pw::system::GetRpcServer().RegisterService(kudzu_service);
  // vTaskStartScheduler();
  // PW_UNREACHABLE;
}

}  // namespace pw::system

// int main() {
//   while (true) {
//     PW_LOG_INFO("Hello, world!");
//     sleep(5);
//   }
//   return 0;
// }
