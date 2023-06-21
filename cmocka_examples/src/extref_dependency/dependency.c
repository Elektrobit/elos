#include "dependency.h"
#include <stdio.h>

int dependency(int a, int b) {
	printf("   genuine: dependency(%d, %d) {\n", a, b);
	printf("      return %d + %d; // = %d\n", a, b, a + b);
	printf("   }\n");
	return a + b;
}
