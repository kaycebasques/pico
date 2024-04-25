#include <array>

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "pw_string/util.h"
#include "pw_system/init.h"
#include "task.h"

namespace {

std::array<StackType_t, configMINIMAL_STACK_SIZE> freertos_idle_stack;
StaticTask_t freertos_idle_tcb;

std::array<StackType_t, configTIMER_TASK_STACK_DEPTH> freertos_timer_stack;
StaticTask_t freertos_timer_tcb;

std::array<char, configMAX_TASK_NAME_LEN> temp_thread_name_buffer;

}  // namespace

extern "C" {

// Functions needed when configGENERATE_RUN_TIME_STATS is on.
void configureTimerForRunTimeStats(void) {}
unsigned long getRunTimeCounterValue(void) {
  return to_ms_since_boot(get_absolute_time());
}

// Required for configCHECK_FOR_STACK_OVERFLOW.
void vApplicationStackOverflowHook(TaskHandle_t, char* pcTaskName) {
  pw::string::Copy(pcTaskName, temp_thread_name_buffer);
  PW_CRASH("Stack OVF for task %s", temp_thread_name_buffer.data());
}

// Required for configUSE_TIMERS.
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
                                    StackType_t** ppxTimerTaskStackBuffer,
                                    uint32_t* pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = &freertos_timer_tcb;
  *ppxTimerTaskStackBuffer = freertos_timer_stack.data();
  *pulTimerTaskStackSize = freertos_timer_stack.size();
}

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = &freertos_idle_tcb;
  *ppxIdleTaskStackBuffer = freertos_idle_stack.data();
  *pulIdleTaskStackSize = freertos_idle_stack.size();
}

int main() {
  stdio_init_all();
  setup_default_uart();
  pw::system::Init();
  vTaskStartScheduler();
  PW_UNREACHABLE;
}

}  // extern "C"
