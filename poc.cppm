#pragma leco tool
export module poc;

import silog;
import sitime;
import sith;

class thread : public sith::thread {
  const char *msg = "Racing condition!";

  void run() override {
    while (!interrupted()) {
      silog::log(silog::info, "%s running", msg);
    }
  }

public:
  thread(const char *msg) : sith::thread{}, msg{msg} {}
};

struct runner {
  int id;

  void log(sith::thread *t) {
    while (!t->interrupted()) {
      silog::log(silog::info, "Thread %d running", id);
    }
  }
};

void run() {
  thread t1{"Thread 1"};
  t1.start();
  thread t2{"Thread 2"};
  sith::run_guard t2g{&t2};

  sith::stateless_thread t3{[](auto) {}}; // Should not emit outputs
  t3.start();

  runner r4{4};
  sith::memfn_thread t4{&r4, &runner::log};
  t4.start();
  runner r5{5};
  sith::memfn_thread t5{&r5, &runner::log};
  t5.start();

  silog::log(silog::info, "Threads started");
  sitime::sleep(1);
}

extern "C" int main() {
  run();
  silog::log(silog::info, "Threads are done");
}
