#pragma once

#include "cerr-log.h"

// ╔═══════════════════════════════[ ASSERTIONS ]══════════════════════════════╗

#ifndef CERR_ASSERT_FDOUT
# define CERR_ASSERT_FDOUT stderr
#endif

#ifndef NDEBUG

# define __LOG_ASSERT(MSG, ...)                                                \
	fprintf(CERR_ASSERT_FDOUT, __F_SEP(__C_RED_B) " > " MSG "\n",              \
		"assert: ", ##__VA_ARGS__)

# define ASSERT(COND, MSG, ...)                                                \
	if (__builtin_expect(!(COND), 0)) {                                        \
		__LOG_ASSERT("Line %d, in %s: Failed, " MSG,                           \
			__LINE__, __FILE__, ##__VA_ARGS__);                                \
		exit(134);                                                             \
	}

#else
# define ASSERT(COND, MSG, ...) ((void)0)
#endif
