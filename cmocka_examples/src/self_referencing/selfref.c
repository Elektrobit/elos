#include "selfref.h"
#include <stdio.h>

int function(int i) {
	printf("genuine: function(%d) {\n", i);
	int res = dependency(i, i);
	printf("}\n");
	return res;
}

int dependency(int a, int b) {
	printf("   genuine: dependency(%d, %d) {\n", a, b);
	printf("      return %d + %d; // = %d\n", a, b, a + b);
	printf("   }\n");
	return a + b;
}
