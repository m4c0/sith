export module sith;
import no;
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
  virtual void run() = 0;

public:
  thread() = default;
  virtual ~thread() noexcept { stop(); }

  thread(const thread &) = delete;
  thread &operator=(const thread &) = delete;

  thread(thread &&o) : m_interrupted{o.m_interrupted}, m_nth{o.m_nth} {
    o.m_nth = nullptr;
  }
  thread &operator=(thread &&o) {
    stop();
    m_nth = o.m_nth;
    m_interrupted = o.m_interrupted;
    o.m_nth = nullptr;
    return *this;
  }

  [[nodiscard]] bool interrupted() const noexcept { return m_interrupted; }

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
export class run_guard : no::no {
  thread *m_t;

public:
  explicit run_guard(thread *t) : m_t{t} { m_t->start(); }
  ~run_guard() { m_t->stop(); }
};

export class stateless_thread : public thread {
  void (*m_run)(thread *);

protected:
  void run() override { m_run(this); }

public:
  stateless_thread() = default;
  stateless_thread(void (*r)(thread *)) : m_run{r} {}
};

export template <typename Tp> class memfn_thread : public thread {
  Tp *m_obj;
  void (Tp::*m_mfn)(thread *);

protected:
  void run() override { (m_obj->*m_mfn)(this); }

public:
  memfn_thread() = default;
  memfn_thread(Tp *o, void (Tp::*m)(thread *)) : m_obj{o}, m_mfn{m} {}
};
export template <typename Tp>
memfn_thread(Tp *, void (Tp::*)(thread *)) -> memfn_thread<Tp>;
} // namespace sith
