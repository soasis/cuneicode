#include <stdint.h>
#include <stdlib.h>

int main() {
	void* p  = malloc(2);
	int conv = (int)(uintptr_t)p;
	free(p);
	return conv;
}
