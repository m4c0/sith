import silog;

namespace sith {
class thread {
  volatile bool m_interrupted{};

  void interrupt() noexcept;

protected:
  [[nodiscard]] bool interrupted() const noexcept { return m_interrupted; }

  virtual void run() = 0;

public:
  thread();
  virtual ~thread() noexcept {
    m_interrupted = true;
    interrupt();
  }
};
} // namespace sith

class thread : public sith::thread {
  void run() override {
    while (!interrupted()) {
      silog::log(silog::info, "Thread is waiting");
    }
  }
};

int main() {
  thread t1{};
  thread t2{};
  silog::log(silog::info, "Threads started");
}
