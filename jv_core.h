#ifndef _JV_CORE_H_INCLUDED_
#define _JV_CORE_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JV_INFTIM -1 /* wait forever */

#define JV_OK 0
#define JV_ERROR -1

#define jv_memzero(buf, n) (void *) memset(buf, 0, n)

#ifndef intptr_t
#define intptr_t long
#define uintptr_t unsigned long
#endif

typedef intptr_t jv_int_t;
typedef uintptr_t jv_uint_t;

typedef unsigned char u_char;

#endif /* _JV_CORE_H_INCLUDED_ */