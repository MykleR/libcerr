#pragma once

#include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define ERR_TLS _Thread_local
# else
  // For older compilers (GCC/Clang) this typically works:
  #define ERR_TLS __thread
# endif

# ifndef UNUSED
# define UNUSED __attribute__((unused))
# endif

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


// ╔═══════════════════════════════[ EXCEPTIONS ]══════════════════════════════╗

# define ERR_MSG_BUF_SIZE 1024

typedef struct sigaction t_err_act;

typedef struct s_err_ctx {
	char					msg[ERR_MSG_BUF_SIZE];
	const char				*file;
	struct s_err_ctx		*prev_ctx;
	t_err_act				prev_act;
	volatile sig_atomic_t	failed;
	int						sig;
	int						stage;
	int						line;
} t_err_ctx;

static ERR_TLS t_err_ctx *g__err_ctx = NULL;

static UNUSED void __err_sig_handler(int UNUSED sig) {
	if (g__err_ctx) g__err_ctx->failed = 1;
}

# define __ERR_CTX_INIT ({ \
	__ctx = (t_err_ctx){0}; \
	t_err_ctx *prev = g__err_ctx; \
	g__err_ctx = &__ctx ; \
	t_err_act action; \
	action = (t_err_act){0}; \
	action.sa_handler = __err_sig_handler; \
    sigaction(SIGUSR1, &action, &(__ctx.prev_act)); \
	(t_err_ctx){.prev_ctx=prev, .prev_act=__ctx.prev_act, .sig=SIGUSR1}; \
})

# define __ERR_CTX_DESTROY ({ \
    sigaction(__ctx.sig, &(__ctx.prev_act), NULL); \
    g__err_ctx = __ctx.prev_ctx; \
})

# define __ERR_MSG_SET(m, ...) do { \
	if (!g__err_ctx) break; \
	g__err_ctx->line = __LINE__; \
	g__err_ctx->file = __FILE__; \
	snprintf(g__err_ctx->msg, ERR_MSG_BUF_SIZE, "Line %d, in %s: "m, __LINE__, __FILE__, ##__VA_ARGS__); \
} while(0)


# define TRY \
    for (t_err_ctx __ctx=__ERR_CTX_INIT; __ctx.stage < 1; __ERR_CTX_DESTROY) \
		for (; __ctx.stage < 2; ++__ctx.stage) \
			if (__builtin_expect(!__ctx.stage, 1)) \
				do

# define CATCH \
				while (0); \
			else if (__builtin_expect(__ctx.failed, 0))

# define THROW_IF(cond, msg, ...) \
	if (__builtin_expect(!(cond), 0)) {\
		__ERR_MSG_SET(msg, ##__VA_ARGS__); \
		raise(SIGUSR1); break;\
	}

