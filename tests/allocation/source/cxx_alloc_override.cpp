#include <version.h>

#include <cstddef>
#include <new>

static void* my_null = 0;

CNC_TESTS_EXPORT void* operator new(size_t size) {
	(void)size;
	return my_null;
}

CNC_TESTS_EXPORT void* operator new[](size_t size) {
	(void)size;
	return my_null;
}

CNC_TESTS_EXPORT void* operator new(size_t size, std::nothrow_t&) {
	(void)size;
	return my_null;
}

CNC_TESTS_EXPORT void* operator new[](size_t size, std::nothrow_t&) {
	(void)size;
	return my_null;
}

CNC_TESTS_EXPORT void operator delete(void* p) noexcept {
	(void)p;
}

CNC_TESTS_EXPORT void operator delete(void* p, size_t size) noexcept {
	(void)p;
	(void)size;
}

CNC_TESTS_EXPORT void operator delete(void* p, std::nothrow_t&) {
	(void)p;
}

CNC_TESTS_EXPORT void operator delete[](void* p) noexcept {
	(void)p;
}

CNC_TESTS_EXPORT void operator delete[](void* p, size_t size) noexcept {
	(void)p;
	(void)size;
}

CNC_TESTS_EXPORT void operator delete[](void* p, std::nothrow_t&) {
	(void)p;
}

CNC_TESTS_EXPORT void* operator new(size_t size, std::align_val_t val) {
	(void)size;
	(void)val;
	return my_null;
}

CNC_TESTS_EXPORT void* operator new[](size_t size, std::align_val_t val) {
	(void)size;
	(void)val;
	return my_null;
}

CNC_TESTS_EXPORT void* operator new(size_t size, std::align_val_t val, std::nothrow_t&) {
	(void)size;
	(void)val;
	return my_null;
}

CNC_TESTS_EXPORT void* operator new[](size_t size, std::align_val_t val, std::nothrow_t&) {
	(void)size;
	(void)val;
	return my_null;
}

CNC_TESTS_EXPORT void operator delete(void* p, std::align_val_t) noexcept {
	(void)p;
}

CNC_TESTS_EXPORT void operator delete[](void* p, std::align_val_t) noexcept {
	(void)p;
}

CNC_TESTS_EXPORT void operator delete(void* p, size_t size, std::align_val_t val) noexcept {
	(void)p;
	(void)size;
	(void)val;
}

CNC_TESTS_EXPORT void operator delete[](void* p, size_t size, std::align_val_t val) noexcept {
	(void)p;
	(void)size;
	(void)val;
}
