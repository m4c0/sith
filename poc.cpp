import silog;
import sith;

class thread : public sith::thread {
  const char *msg = "Racing condition!";

  void run() override {
    while (!interrupted()) {
      silog::log(silog::info, msg);
    }
  }

public:
  thread(const char *msg) : sith::thread{}, msg{msg} {}
};

void run() {
  thread t1{"Thread 1 running"};
  thread t2{"Thread 2 running"};
  silog::log(silog::info, "Threads started");
}

int main() {
  run();
  silog::log(silog::info, "Threads are done");
}
