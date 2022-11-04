#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include "..\LM\include\levmar.h"
#ifdef _cplusplus
extern "C" {
#include "..\LM\include\f2c.h"
#include "..\LM\include\clapack.h"
}
#endif

#ifndef LM_DBL_PREC
#error Example program assumes that levmar has been compiled with double precision, see LM_DBL_PREC!
#endif

// the following macros concern the initialization of a random number generator for adding noise
#undef REPEATABLE_RANDOM /* #define this for repeatable program behavior across runs */

// 定义宏:GETPID
#ifdef _MSC_VER // MSVC
#include <process.h>
#define GETPID  _getpid
#elif defined(__GNUC__) // GCC
#include <sys/types.h>
#include <unistd.h>
#define GETPID  getpid
#else
#warning Do not know the name of the function returning the process id
#define GETPID  0
#endif /* _MSC_VER */

// 定义宏:INIT_RANDOM(seed)
#ifdef REPEATABLE_RANDOM
#define INIT_RANDOM(seed) srandom(seed)
#else
#define INIT_RANDOM(seed) srand((int)GETPID()) // seed unused
#endif