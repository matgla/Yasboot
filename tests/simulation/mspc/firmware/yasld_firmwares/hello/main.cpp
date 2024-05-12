#include "pico/stdlib.h"
#include <cstdio>

extern "C"
{
  extern char __attribute__((section(".data"))) * __StackTop;
}

int main()
{
  stdio_init_all();
  while (true)
  {
    printf("Hello from standalone relocatable module!\n");
    sleep_ms(500);
  }
}
