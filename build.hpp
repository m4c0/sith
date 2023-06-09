#pragma once
#include "../silog/build.hpp"
#include "../traits/build.hpp"
#include "ecow.hpp"

auto sith() {
  using namespace ecow;

  auto m = unit::create<mod>("sith");
  m->add_wsdep("silog", silog());
  m->add_wsdep("traits", traits());
  m->add_unit<objc>("apple");
  m->add_unit<>("droid")->add_requirement(android_ndk);
  m->add_unit<>("windows")->add_requirement(windows_api);
  m->add_requirement(native);
  return m;
}
