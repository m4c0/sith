export module sith;
import silog;
import traits;

#if __APPLE__
#pragma leco add_impl pthread
#elif __ANDROID__
#pragma leco add_impl pthread
#elif _WIN32
#pragma leco add_impl windows
#else
#error unsupported
#endif

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

  void *create();
  void destroy(void *);

protected:
  [[nodiscard]] bool interrupted() const noexcept { return m_interrupted; }

  virtual void run() = 0;

public:
  thread() = default;
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
      m_nth = create();
  }
  void stop() {
    m_interrupted = true;
    if (m_nth != nullptr) {
      destroy(m_nth);
      m_nth = nullptr;
    }
  }
};
} // namespace sith
