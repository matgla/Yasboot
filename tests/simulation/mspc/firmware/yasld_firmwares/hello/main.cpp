#include "pico/stdlib.h"

#include <cstdio>

#include "vectors.hpp"

extern "C"
{
  extern uint32_t app_stack_pointer;
  extern char __attribute__((section(".data"))) * __StackTop;
  void runtime_init();
}

int main()
{
  initialize_default_vector_table();
  asm volatile("msr msp, %0" : : "r"(app_stack_pointer) :);
  runtime_init();
  stdio_init_all();
  while (true)
  {
    printf("Hello from standalone relocatable module!\n");
    sleep_ms(500);
  }
}
