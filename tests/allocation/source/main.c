#include <stdint.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	void* p  = malloc(2);
	int conv = (int)(uintptr_t)p;
	free(p);
	return conv;
}
