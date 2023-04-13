#include "../silog/build.hpp"
#include "build.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto poc = unit::create<app>("poc");
  poc->add_wsdep("silog", silog());
  poc->add_unit<>("poc");
  return run_main(poc, argc, argv);
}
