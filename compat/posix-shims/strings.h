#include <compat.h>

#ifndef HAVE_STRCASECMP
#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif
#endif
