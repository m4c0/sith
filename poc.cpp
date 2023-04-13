import silog;
import sith;

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
