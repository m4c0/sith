#include <pthread.h>
#include <stdlib.h>

struct wrap {
  void (*fn)(void *);
  void *data;
};
void *void_start_wrap(void *wrapped) {
  wrap *w = static_cast<wrap *>(wrapped);
  w->fn(w->data);
  delete w;
  return nullptr;
}

extern "C" void *sith_create(void *data, void (*fn)(void *)) {
  pthread_t *res = new pthread_t;
  if (0 != pthread_create(res, nullptr, void_start_wrap, new wrap{fn, data})) {
    exit(-1);
  }
  return res;
}
extern "C" void sith_destroy(void *nth) {
  pthread_t *t = static_cast<pthread_t *>(nth);
  pthread_join(*t, nullptr);
  delete t;
}
