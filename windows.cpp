#include <windows.h>

extern "C" void *sith_create(void *data, void (*fn)(void *)) {
  return CreateThread(nullptr, 0, fn, data,
                      0, // CREATE_SUSPENDED / ResumeThread
                      nullptr);
}
extern "C" void sith_destroy(void *nth) {
  WaitForSingleObject((HANDLE)nth, INFINITY);
}
