#pragma once

# include <stdio.h>

// ╔════════════════════════════════[ LOGGING ]═══════════════════════════════╗

# define __LOG_LEVELS 4

# ifndef LOG_FDOUT
#  define LOG_FDOUT stderr
# endif

# ifndef LOG_LEVEL
#  define LOG_LEVEL __LOG_LEVELS
# endif

# define 	__F_COLOR(C, X)		C X __F_RESET
# define	__F_SEP(C)			__F_BOLD(__F_COLOR(C, "%10s"))
# define	__F_BOLD(X)			"\033[1m" X "\033[22m"
# define 	__F_ITALIC(X)		"\033[3m" X "\033[23m"
# define	__F_RESET			"\033[0m"
# define	__C_RED				"\033[31m"
# define	__C_GREEN			"\033[32m"
# define	__C_YELLOW			"\033[33m"
# define	__C_BLUE			"\033[34m"
# define	__C_MAGENTA			"\033[35m"
# define	__C_CYAN			"\033[36m"
# define	__C_GRAY			"\033[90m"

# ifndef NVERBOSE
#  define __LOG(COLOR, TITLE, MSG, ...)	\
	fprintf(LOG_FDOUT, __F_SEP(COLOR) " > " MSG "\n", TITLE, ##__VA_ARGS__)

#  define LOG_NL() \
	fprintf(LOG_FDOUT, "\n")
# else
#  define __LOG(COLOR, TITLE, MSG, ...)	((void)0)
#  define LOG_NL()						((void)0)
# endif

# if LOG_LEVEL >= __LOG_LEVELS
#  define LOG_DEBUG(MSG, ...) __LOG(__C_BLUE, "debug: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_DEBUG(MSG, ...) ((void)0)
# endif

# if LOG_LEVEL >= __LOG_LEVELS - 1
#  define LOG_INFO(MSG, ...) __LOG(__C_CYAN, "info: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_INFO(MSG, ...) ((void)0)
# endif

# if LOG_LEVEL >= __LOG_LEVELS - 2
#  define LOG_WARN(MSG, ...) __LOG(__C_YELLOW, "warning: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_WARN(MSG, ...) ((void)0)
# endif

# if LOG_LEVEL >= __LOG_LEVELS - 3
#  define LOG_OK(MSG, ...) __LOG(__C_GREEN, "done: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_OK(MSG, ...) ((void)0)
# endif

# if LOG_LEVEL >= __LOG_LEVELS - 3
#  define LOG_ERR(MSG, ...) __LOG(__C_RED, "error: ", MSG, ##__VA_ARGS__)
# else
#  define LOG_ERR(MSG, ...) ((void)0)
# endif
