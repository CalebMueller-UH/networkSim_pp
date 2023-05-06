#include "Color.hpp"

#include <stdarg.h>
#include <stdio.h>

#include <string>

#include "Color.hpp"

void colorPrint(color c, const char* format, ...) {
  va_list args;
  va_start(args, format);

  char buffer[1024];
  int len = colorVsnprintf(buffer, sizeof(buffer), c, format, args);

  fwrite(buffer, 1, len, stdout);
  va_end(args);
}

int colorVsnprintf(char* str, size_t size, color c, const char* format,
                   va_list args) {
  const ColorCode& color_code = color_codes[c];
  int written = snprintf(str, size, "%s", color_code.code);

  va_list args_copy;
  va_copy(args_copy, args);
  written += vsnprintf(str + written, size - written, format, args_copy);
  va_end(args_copy);

  written += snprintf(str + written, size - written, "%s", color_code.reset);

  return written;
}