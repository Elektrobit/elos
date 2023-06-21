#include <stdio.h>

#include "selfref.h"
#include "mock_selfref.h"

int main() {
	MOCK_FUNC_AFTER_CALL(dependency, 0);
	int res = function(2);
	printf(" -> %d // %s\n", res, 0 == res ? "correctly mocked!" : "not mocked!");
}
