export module poc;

import silog;
import sitime;
import sith;

class thread : public sith::thread {
  const char *msg = "Racing condition!";

  void run() override {
    while (!interrupted()) {
      silog::log(silog::info, "%s", msg);
    }
  }

public:
  thread(const char *msg) : sith::thread{}, msg{msg} { start(); }
};

class dummy_thread : public sith::thread {
  void run() override {}
};

void run() {
  thread t1{"Thread 1 running"};
  thread t2{"Thread 2 running"};
  dummy_thread t3{}; // Should not emit outputs
  silog::log(silog::info, "Threads started");
  sitime::sleep(1);
}

int main() {
  run();
  silog::log(silog::info, "Threads are done");
}
