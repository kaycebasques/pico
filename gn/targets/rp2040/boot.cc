#include <stdio.h>
#include <array>

// # include "pw_system/rpc_server.h"

// #define PW_LOG_MODULE_NAME "pw_system"

// #include "FreeRTOS.h"
// # include "kudzu/kudzu_service_pwpb.h"
#include "pico/stdlib.h"
// #include "pw_log/log.h"
// #include "pw_string/util.h"
// #include "pw_system/init.h"
// #include "task.h"

// namespace {
// 
// std::array<StackType_t, configMINIMAL_STACK_SIZE> freertos_idle_stack;
// StaticTask_t freertos_idle_tcb;
// 
// std::array<StackType_t, configTIMER_TASK_STACK_DEPTH> freertos_timer_stack;
// StaticTask_t freertos_timer_tcb;
// 
// std::array<char, configMAX_TASK_NAME_LEN> temp_thread_name_buffer;
// 
// }  // namespace

extern "C" {

// // Functions needed when configGENERATE_RUN_TIME_STATS is on.
// void configureTimerForRunTimeStats(void) {}
// unsigned long getRunTimeCounterValue(void) {
//   return to_ms_since_boot(get_absolute_time());
// }
// 
// // Required for configCHECK_FOR_STACK_OVERFLOW.
// void vApplicationStackOverflowHook(TaskHandle_t, char* pcTaskName) {
//   pw::string::Copy(pcTaskName, temp_thread_name_buffer);
//   PW_CRASH("Stack OVF for task %s", temp_thread_name_buffer.data());
// }
// 
// // Required for configUSE_TIMERS.
// void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
//                                     StackType_t** ppxTimerTaskStackBuffer,
//                                     uint32_t* pulTimerTaskStackSize) {
//   *ppxTimerTaskTCBBuffer = &freertos_timer_tcb;
//   *ppxTimerTaskStackBuffer = freertos_timer_stack.data();
//   *pulTimerTaskStackSize = freertos_timer_stack.size();
// }
// 
// void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
//                                    StackType_t** ppxIdleTaskStackBuffer,
//                                    uint32_t* pulIdleTaskStackSize) {
//   *ppxIdleTaskTCBBuffer = &freertos_idle_tcb;
//   *ppxIdleTaskStackBuffer = freertos_idle_stack.data();
//   *pulIdleTaskStackSize = freertos_idle_stack.size();
// }

// kudzu::rpc::KudzuService kudzu_service;

int main() {
  #pragma message "can print compiler messages with this..."
  // PICO_SDK Inits
  stdio_init_all();
  setup_default_uart();
  const uint LED_PIN = 25;
  const uint WAIT_MS = 1000;
  gpio_init(LED_PIN);
  while (1) {
    printf("Hello, world!\n");
    gpio_put(LED_PIN, 1);
    sleep_ms(WAIT_MS);
    gpio_put(LED_PIN, 0);
    sleep_ms(WAIT_MS);
  }
  // stdio_usb_init();
  // PW_LOG_INFO("pw_system main");
  // pw::system::Init();
  // pw::system::GetRpcServer().RegisterService(kudzu_service);
  // vTaskStartScheduler();
  // PW_UNREACHABLE;
}

}  // extern "C"
