#pragma once

#include <libcerr-exception.h>

// ╔═══════════════════════════════[ ASSERTIONS ]══════════════════════════════╗

#ifndef NDEBUG
# define __LOG_ASSERT(MSG, ...)	                                               \
	fprintf(LOG_FDOUT, __F_SEP(__C_RED) " > " MSG "\n", "assert: ", ##__VA_ARGS__)

# define ASSERT(COND, MSG, ...)                                                \
	if (__builtin_expect(!(COND), 0)) {                                        \
		THROW_MSG(CERR_E_ASSERT, MSG, ##__VA_ARGS__);                          \
		__LOG_ASSERT("Failed line %d, in %s: " MSG,                            \
			__LINE__, __FILE__, ##__VA_ARGS__);                                \
		exit(134);                                                             \
	}
#else
# define ASSERT(COND, MSG, ...) ((void)0)
#endif
