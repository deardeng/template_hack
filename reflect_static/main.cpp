#include <iostream>
#include <string>
#include <cstdint>

#define message_name foo
#define message_body  \
  field_int64(i64, 123) \
  field_double(dbl) \
  field_string(str, "hello")  \
  //
#include "message_generator.hpp"



int main() {
  foo f;
  std::cout << f << std::endl;
  return 0;
}
