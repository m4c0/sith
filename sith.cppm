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

namespace sith {
export class thread {
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
