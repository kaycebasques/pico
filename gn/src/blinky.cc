#include <unistd.h>

#define LIB_CMSIS_CORE 0

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
// pw_digital_io_rp2040 docs should mention you don't need to explicitly
// depend on pw_digital_io because it's provided through public deps
#include "pw_digital_io/polarity.h"
#include "pw_digital_io_rp2040/digital_io.h"
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
  constexpr pw::digital_io::Rp2040Config led_pin_config{
    .pin = 25,
    .polarity = pw::digital_io::Polarity::kActiveHigh
  };
  pw::digital_io::Rp2040DigitalInOut led_pin(led_pin_config);
  led_pin.Enable();
  const uint WAIT_MS = 1000;
  while (1) {
    led_pin.SetState(pw::digital_io::State::kActive);
    sleep_ms(WAIT_MS);
    led_pin.SetState(pw::digital_io::State::kInactive);
    sleep_ms(WAIT_MS);
  }
}

}  // namespace

namespace pw::system {

void UserAppInit() {
  pw::thread::DetachedThread(DisplayDrawThreadOptions(), MainTask);
}

}  // namespace pw::system
