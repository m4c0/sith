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
  // It isn't always desirable to start a thread in its ctor and stopping them
  // in the dtor might lead to threads still running while child attributes were
  // destroyed. Therefore, it is better to RAII-ify the execution of a thread.
  // This is similar to mutex and its locks.
  thread t1{"Thread 1"};
  sith::run_guard t1g{&t1};
  thread t2{"Thread 2"};
  sith::run_guard t2g{&t2};

  sith::stateless_thread t3{[](auto) {}}; // Should not emit outputs
  sith::run_guard t3g{&t3};

  runner r4{4};
  sith::memfn_thread t4{&r4, &runner::log};
  sith::run_guard t4g{&t4};
  runner r5{5};
  sith::memfn_thread t5{&r5, &runner::log};
  sith::run_guard t5g{&t5};

  silog::log(silog::info, "Threads started");
  sitime::sleep(1);
}

extern "C" int main() {
  run();
  silog::log(silog::info, "Threads are done");
}
