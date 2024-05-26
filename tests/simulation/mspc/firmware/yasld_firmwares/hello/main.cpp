#include "pico/stdlib.h"

#include <cstdio>

#include "vectors.hpp"

extern "C"
{
  extern uint32_t app_stack_pointer;
  //  char *__StackTop = reinterpret_cast<char *>(app_stack_pointer);
  void runtime_init();
  void initialize_on_entry();
}

int main()
{
  initialize_on_entry();
  runtime_init();
  stdio_init_all();
  while (true)
  {
    printf("Hello from standalone relocatable module!\n");
    sleep_ms(500);
  }
}
