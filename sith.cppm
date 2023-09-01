export module sith;
import silog;
import traits;

#if __APPLE__
#pragma ecow add_impl pthread
#elif __ANDROID__
#pragma ecow add_impl pthread
#elif _WIN32
#pragma ecow add_impl windows
#else
#error unsupported
#endif

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
  virtual ~thread() noexcept { stop(); }

  thread(const thread &) = delete;
  thread &operator=(const thread &) = delete;

  thread(thread &&o) : m_nth{o.m_nth}, m_interrupted{o.m_interrupted} {
    o.m_nth = nullptr;
  }
  thread &operator=(thread &&o) {
    stop();
    m_nth = o.m_nth;
    m_interrupted = o.m_interrupted;
    o.m_nth = nullptr;
    return *this;
  }

  void start() {
    if (m_nth == nullptr)
      m_nth = sith_create(this, &thread::callback);
  }
  void stop() {
    m_interrupted = true;
    if (m_nth != nullptr) {
      sith_destroy(m_nth);
      m_nth = nullptr;
    }
  }
};
} // namespace sith
