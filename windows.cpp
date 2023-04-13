#include <windows.h>

struct wrap {
  void (*fn)(void *);
  void *data;
};
DWORD void_start_wrap(void *wrapped) {
  wrap *w = static_cast<wrap *>(wrapped);
  w->fn(w->data);
  delete w;
  return 0;
}

extern "C" void *sith_create(void *data, void (*fn)(void *)) {
  return CreateThread(nullptr, 0, void_start_wrap, new wrap{fn, data},
                      0, // CREATE_SUSPENDED / ResumeThread
                      nullptr);
}
extern "C" void sith_destroy(void *nth) {
  WaitForSingleObject((HANDLE)nth, INFINITE);
}
