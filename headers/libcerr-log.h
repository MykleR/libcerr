#pragma once

# include <stdio.h>

// ╔════════════════════════════════[ LOGGING ]═══════════════════════════════╗

# ifndef LOG_FDOUT
#  define LOG_FDOUT stderr
# endif

# define 	__F_COLOR(C, X)		C X __F_RESET
# define	__F_SEP(C)			__F_BOLD(__F_COLOR(C, "%8s"))
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
#  define __LOG(c, t, msg, ...)	\
	fprintf(LOG_FDOUT, __F_SEP(c) " > " msg "\n", t, ##__VA_ARGS__)

#  define LOG_NL() \
	fprintf(LOG_FDOUT, "\n")
# else
#  define __LOG(c, t, msg, ...)	((void)0)
#  define LOG_NL()				((void)0)
# endif

# define LOG_DEBUG(msg, ...)	__LOG(__C_BLUE,		"debug: ", msg, ##__VA_ARGS__)
# define LOG_INFO(msg, ...)		__LOG(__C_CYAN,      "info: ", msg, ##__VA_ARGS__)
# define LOG_WARN(msg, ...)		__LOG(__C_YELLOW, "warning: ", msg, ##__VA_ARGS__)
# define LOG_OK(msg, ...)		__LOG(__C_GREEN,	 "done: ", msg, ##__VA_ARGS__)
# define LOG_ERR(msg, ...)		__LOG(__C_RED,		"error: ", msg, ##__VA_ARGS__)
