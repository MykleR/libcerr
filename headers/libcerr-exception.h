#pragma once

# include <stdint.h>
# include <signal.h>
# include <stdlib.h>

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define ERR_TLS _Thread_local
# else
  // For older compilers (GCC/Clang) this typically works:
  #define ERR_TLS __thread
# endif

# ifndef UNUSED
#  define UNUSED __attribute__((unused))
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
