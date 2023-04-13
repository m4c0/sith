#include "build.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto poc = unit::create<app>("poc");
  return run_main(poc, argc, argv);
}
