#pragma once

# include <stdio.h>

// ╔═══════════════════════════════[ ASSERTIONS ]══════════════════════════════╗

# ifndef NDEBUG
#  define __LOG_ASSERT(msg, ...)	\
	fprintf(LOG_FDOUT, __F_SEP(__C_RED) " > " msg "\n", "assert: ", ##__VA_ARGS__)

#  define ASSERT(cond, msg, ...) \
	if (__builtin_expect(!(cond), 0)) { \
		__LOG_ASSERT("Failed line %d, in %s: " msg, __LINE__, __FILE__, ##__VA_ARGS__); \
		exit(134); }
# else
#  define ASSERT_MSG(cond, msg, ...) ((void)0)
# endif
