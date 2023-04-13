#pragma once
#include "../traits/build.hpp"
#include "ecow.hpp"

auto sith() {
  using namespace ecow;

  auto m = unit::create<mod>("sith");
  m->add_wsdep("traits", traits());
  return m;
}
