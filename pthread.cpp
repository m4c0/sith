module;
#include <pthread.h>
#include <stdlib.h>

module sith;

void *sith::thread::create() {
  static const auto callback = [](void *x) -> void * {
    sith::thread::callback(x);
    return nullptr;
  };

  pthread_t *res = new pthread_t;
  if (0 != pthread_create(res, nullptr, callback, this)) {
    exit(-1);
  }
  return res;
}
void sith::thread::destroy(void *nth) {
  pthread_t *t = static_cast<pthread_t *>(nth);
  pthread_join(*t, nullptr);
  delete t;
}

void sith::thread::set_name(const char * n) const {
  pthread_setname_np(n);
}
