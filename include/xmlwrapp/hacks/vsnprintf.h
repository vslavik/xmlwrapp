#ifndef XMLWRAPP_HACKS_VSNPRINTF_H_20040725T151926
#define XMLWRAPP_HACKS_VSNPRINTF_H_20040725T151926

#ifdef _MSC_VER
#  include <cstdio>
#  undef vsnprintf
#  define vsnprintf _vsnprintf
#endif

#endif
