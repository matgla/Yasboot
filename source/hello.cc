module;

#include <cstdio>
#include <string_view>

export module hello;
export void greet(std::string_view name)
{
  printf("Hello %s\n", name.data());
}
