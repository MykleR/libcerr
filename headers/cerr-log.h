#pragma once

# include <stdio.h>
#include "cerr-colors.h"

// ╔════════════════════════════════[ LOGGING ]═══════════════════════════════╗

# define __LOG_LEVELS 4

# ifndef CERR_LOG_FDOUT
#  define CERR_LOG_FDOUT stderr
# endif

# ifndef CERR_LOG_LEVEL
#  define CERR_LOG_LEVEL __LOG_LEVELS
# endif

# define 	$COLOR(C, X)	C X $
# define	$BOLD(X)		$b X $n
# define 	$ITALIC(X)		$i X $ni

# define	__SEP(C)		$BOLD($COLOR(C, "%10s"))

# ifndef NVERBOSE
#  define __LOG(COLOR, TITLE, MSG, ...)                                        \
	fprintf(CERR_LOG_FDOUT, __SEP(COLOR) " > " MSG "\n", TITLE, ##__VA_ARGS__)

#  define LOG_NL()                                                             \
	fprintf(CERR_LOG_FDOUT, "\n")

#  define LOG_IF(COND, LOG, ...)                                               \
	if (COND) LOG(__VA_ARGS__)
# else
#  define __LOG(COLOR, TITLE, MSG, ...)	((void)0)
#  define LOG_NL()						((void)0)
#  define LOG_IF()						((void)0)
# endif

# if CERR_LOG_LEVEL >= __LOG_LEVELS
#  define LOG_DEBUG(MSG, ...) __LOG($blue, "debug: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_DEBUG(MSG, ...) ((void)0)
# endif

# if CERR_LOG_LEVEL >= __LOG_LEVELS - 1
#  define LOG_INFO(MSG, ...) __LOG($cyan, "info: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_INFO(MSG, ...) ((void)0)
# endif

# if CERR_LOG_LEVEL >= __LOG_LEVELS - 2
#  define LOG_WARN(MSG, ...) __LOG($yellow, "warning: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_WARN(MSG, ...) ((void)0)
# endif

# if CERR_LOG_LEVEL >= __LOG_LEVELS - 3
#  define LOG_OK(MSG, ...) __LOG($green, "done: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_OK(MSG, ...) ((void)0)
# endif

# if CERR_LOG_LEVEL >= __LOG_LEVELS - 3
#  define LOG_ERR(MSG, ...) __LOG($red, "error: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_ERR(MSG, ...) ((void)0)
# endif

#define LOG_IF_DEBUG(COND, ...) LOG_IF(COND, LOG_DEBUG, __VA_ARGS__)
#define LOG_IF_INFO(COND, ...)	LOG_IF(COND, LOG_INFO, __VA_ARGS__)
#define LOG_IF_WARN(COND, ...)	LOG_IF(COND, LOG_WARN, __VA_ARGS__)
#define LOG_IF_ERR(COND, ...)	LOG_IF(COND, LOG_ERR, __VA_ARGS__)
#define LOG_IF_OK(COND, ...)	LOG_IF(COND, LOG_OK, __VA_ARGS__)
