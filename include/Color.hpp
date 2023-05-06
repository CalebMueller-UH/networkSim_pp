#pragma once

#include <string>

typedef enum {
  RED,
  BOLD_RED,
  ORANGE,
  BOLD_ORANGE,
  YELLOW,
  BOLD_YELLOW,
  GREEN,
  BOLD_GREEN,
  CYAN,
  BOLD_CYAN,
  BLUE,
  BOLD_BLUE,
  MAGENTA,
  BOLD_MAGENTA,
  PURPLE,
  BOLD_PURPLE,
  GREY,
  BOLD_GREY
} color;

struct ColorCode {
  const char* code;
  const char* reset;
};

const int NUM_COLORS = BOLD_GREY + 1;

static const ColorCode color_codes[NUM_COLORS] = {
    {"\033[0;31m", "\033[0m"},        // RED
    {"\033[1;31m", "\033[0m"},        // BOLD_RED
    {"\033[0;33m", "\033[0m"},        // ORANGE
    {"\033[1;38;5;208m", "\033[0m"},  // BOLD_ORANGE
    {"\033[0;33m", "\033[0m"},        // YELLOW
    {"\033[1;33m", "\033[0m"},        // BOLD_YELLOW
    {"\033[0;32m", "\033[0m"},        // GREEN
    {"\033[1;32m", "\033[0m"},        // BOLD_GREEN
    {"\033[0;36m", "\033[0m"},        // CYAN
    {"\033[1;36m", "\033[0m"},        // BOLD_CYAN
    {"\033[0;34m", "\033[0m"},        // BLUE
    {"\033[1;34m", "\033[0m"},        // BOLD_BLUE
    {"\033[0;35m", "\033[0m"},        // MAGENTA
    {"\033[1;35m", "\033[0m"},        // BOLD_MAGENTA
    {"\033[0;35m", "\033[0m"},        // PURPLE
    {"\033[1;35m", "\033[0m"},        // BOLD_PURPLE
    {"\033[0;90m", "\033[0m"},        // GREY
    {"\033[1;90m", "\033[0m"},        // BOLD_GREY
};

void colorPrint(color c, const char* format, ...);

int colorVsnprintf(char* str, size_t size, color c, const char* format,
                   va_list args);