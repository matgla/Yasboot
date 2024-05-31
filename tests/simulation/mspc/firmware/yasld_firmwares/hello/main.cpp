#include "pico/stdlib.h"

#include <cstdio>

int main()
{
  stdio_init_all();
  int counter = 0;
  while (true)
  {
    ++counter;
    printf("Hello from standalone relocatable module %d!\n", counter);
    sleep_ms(500);
  }
}
