#include "../silog/build.hpp"
#include "build.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto poc = unit::create<app>("poc");
  poc->add_wsdep("silog", silog());
  poc->add_ref(sith());
  poc->add_unit<>("poc");
  poc->add_requirement(native);
  return run_main(poc, argc, argv);
}
