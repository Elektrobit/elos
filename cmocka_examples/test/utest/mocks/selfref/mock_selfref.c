#include "mock_selfref.h"

#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>

MOCK_FUNC_VAR_NEW(function);
int MOCK_FUNC_WRAP(function)(int i) {
	return MOCK_FUNC_REAL(function)(i);
}

MOCK_FUNC_BODY(dependency, int, int a, int b) {
	if (MOCK_IS_ACTIVE(dependency)) {
		printf("   mocked: dependency(%d, %d) {\n", a, b);
		printf("      return %d - %d; // = %d\n", a, b, a - b);
		printf("   }\n");
		return a - b;
	}
	return MOCK_FUNC_REAL(dependency)(a, b);
}

