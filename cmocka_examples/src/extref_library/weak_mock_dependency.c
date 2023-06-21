#include <stdio.h>

#include "dependency.h"

int dependency(int a, int b) {
  printf("   mock: dependency(%d, %d) {\n", a, b);
  printf("      return %d - %d; // = %d\n", a, b, a - b);
  printf("   }\n");
  return a - b;
}
