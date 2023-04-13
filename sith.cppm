export module sith;
import silog;
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

  static void callback(void *self) {
    try {
      silog::log(silog::info, "Background thread started");
      static_cast<thread *>(self)->run();
      silog::log(silog::info, "Background thread ended");
    } catch (...) {
      // Just a catch-all. Users should implement their own error handling
      silog::log(silog::error, "Unexpected failure in background thread");
    }
  }

protected:
  [[nodiscard]] bool interrupted() const noexcept { return m_interrupted; }

  virtual void run() = 0;

public:
  thread(bool should_start = true) {
    if (should_start)
      start();
  }
  virtual ~thread() noexcept {
    m_interrupted = true;

    if (m_nth != nullptr)
      sith_destroy(m_nth);
  }

  void start() {
    if (m_nth == nullptr)
      m_nth = sith_create(this, &thread::callback);
  }
};
} // namespace sith
