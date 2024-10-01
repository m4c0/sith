module;
#include <windows.h>

module sith;

void *sith::thread::create() {
  static const auto callback = [](void *x) -> DWORD {
    sith::thread::callback(x);
    return 0;
  };

  return CreateThread(nullptr, 0, callback, this,
                      0, // CREATE_SUSPENDED / ResumeThread
                      nullptr);
}
void sith::thread::destroy(void *nth) {
  WaitForSingleObject((HANDLE)nth, INFINITE);
  CloseHandle((HANDLE)nth);
}
void sith::thread::set_name(const char * n) const {
  size_t count{};

  wchar_t wfn[MAX_PATH + 1];
  mbstowcs_s(&count, wfn, strlen(n), n, _TRUNCATE);
  wfn[count + 1] = 0;

  SetThreadDescription((HANDLE)nth, wfn);
}
