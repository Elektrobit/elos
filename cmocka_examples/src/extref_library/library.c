#include "library.h"
#include "dependency.h"
#include <stdio.h>

int function(int i) {
	printf("genuine: function(%d) {\n", i);
	int res = dependency(i, i);
	printf("}\n");
	return res;
}

