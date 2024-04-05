#include "Util.h"
#include <cstdio>
#include <cstdlib>

void ErrorIf(bool condition, const char *msg) {
  if (condition) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}
