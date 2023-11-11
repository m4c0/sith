#pragma leco tool
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

void run() {
  thread t1{"Thread 1 running"};
  thread t2{"Thread 2 running"};
  sith::stateless_thread t3{[] {}}; // Should not emit outputs
  silog::log(silog::info, "Threads started");
  sitime::sleep(1);
}

extern "C" int main() {
  run();
  silog::log(silog::info, "Threads are done");
}
