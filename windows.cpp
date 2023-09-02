module;
#include <windows.h>

module sith;

void *sith::thread::create() {
  static const auto callback = [](void *x) -> DWORD {
    sith::thread::callback(x);
    return 0;
  };

  return CreateThread(nullptr, 0, void_start_wrap, callback,
                      0, // CREATE_SUSPENDED / ResumeThread
                      nullptr);
}
void sith::thread::destroy(void *nth) {
  WaitForSingleObject((HANDLE)nth, INFINITE);
}
