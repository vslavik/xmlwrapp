#ifndef XMLWRAPP_HACKS_NSSTD_H_20040725T152020
#define XMLWRAPP_HACKS_NSSTD_H_20040725T152020

#if _MSC_VER / 100 == 12
#  include <cstddef>
#  include <cstring>
namespace std {
  using ::size_t;
  using ::ptrdiff_t;
  using ::memset;
  using ::strlen;
}
#endif

#endif
