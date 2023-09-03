#include "build.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto poc = unit::create<app>("poc");
  poc->add_unit<box>("poc");
  poc->add_requirement(native);
  return run_main(poc, argc, argv);
}
