export module sith;
import traits;

// This is only needed until clang fixes a linking bug. Without these, anything
// based on hai fails because of missing "hai::operator X"
extern "C" void *malloc(traits::size_t);
extern "C" void free(void *);
void *operator new(traits::size_t count) { return malloc(count); }
void *operator new[](traits::size_t count) { return malloc(count); }
void operator delete(void *ptr) noexcept { return free(ptr); }
void operator delete[](void *ptr) noexcept { return free(ptr); }

extern "C" void *sith_create(void *data, void (*fn)(void *));
extern "C" void sith_destroy(void *nth);

namespace sith {
export class thread {
  volatile bool m_interrupted{};
  void *m_nth{};

  static void callback(void *self) { static_cast<thread *>(self)->run(); }

protected:
  [[nodiscard]] bool interrupted() const noexcept { return m_interrupted; }

  void start() { m_nth = sith_create(this, &thread::callback); }
  virtual void run() = 0;

public:
  thread() { start(); }
  virtual ~thread() noexcept {
    m_interrupted = true;
    sith_destroy(m_nth);
  }
};
} // namespace sith
