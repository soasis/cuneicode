#include <version.h>

#include <stddef.h>

CNC_TESTS_EXPORT void* __malloc_end_pointer(void* ptr) {
	(void)ptr;
	return 0;
}

CNC_TESTS_EXPORT void* malloc(size_t size) {
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT void free(void* ptr) {
	(void)ptr;
}

CNC_TESTS_EXPORT void cfree(void* ptr) {
	(void)ptr;
}

CNC_TESTS_EXPORT void* calloc(size_t nmemb, size_t size) {
	(void)nmemb;
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT
size_t malloc_usable_size(void* ptr) {
	(void)ptr;
	return 0;
}

CNC_TESTS_EXPORT
size_t malloc_good_size(size_t size) {
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT void* realloc(void* ptr, size_t size) {
	(void)ptr;
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT void* reallocarray(void* ptr, size_t nmemb, size_t size) {
	(void)ptr;
	(void)nmemb;
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT int reallocarr(void* ptr, size_t nmemb, size_t size) {
	(void)ptr;
	(void)nmemb;
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT void* memalign(size_t alignment, size_t size) {
	(void)size;
	(void)alignment;
	return 0;
}

CNC_TESTS_EXPORT void* aligned_alloc(size_t alignment, size_t size) {
	(void)size;
	(void)alignment;
	return 0;
}

CNC_TESTS_EXPORT int posix_memalign(void** memptr, size_t alignment, size_t size) {
	(void)memptr;
	(void)size;
	(void)alignment;
	return 0;
}

CNC_TESTS_EXPORT void* valloc(size_t size) {
	(void)size;
	return 0;
}

CNC_TESTS_EXPORT void* pvalloc(size_t size) {
	(void)size;
	return 0;
}
