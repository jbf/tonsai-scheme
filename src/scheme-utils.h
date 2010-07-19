#ifndef _SCHEME_UTILS_H
#define _SCHEME_UTILS_H 1

#include "cell.h"

#define CADR(c) CAR(CDR(c))
#define CADDR(c) CAR(CDR(CDR(c)))

#endif /* _SCHEME_UTILS_H */
