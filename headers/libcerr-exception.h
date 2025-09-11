#pragma once

# include <stdint.h>
# include <stdlib.h>
# include <setjmp.h>

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define ERR_TLS _Thread_local
# else
  // For older compilers (GCC/Clang) this typically works:
  #define ERR_TLS __thread
# endif

// ╔═══════════════════════════════[ EXCEPTIONS ]══════════════════════════════╗

# define ERR_MSG_BUF_SIZE 1024

typedef struct s_err_ctx {
	struct s_err_ctx	*prev_ctx;
	const char			*file;
	char				msg[ERR_MSG_BUF_SIZE];
	int					thrown;
	int					stage;
	int					line;
} t_err_ctx;

static ERR_TLS t_err_ctx *g__err_ctx = NULL;


# define __ERR_INIT ({ \
	t_err_ctx *prev = g__err_ctx; \
	g__err_ctx = &__err ; \
	(t_err_ctx){.prev_ctx=prev, .file=NULL, .msg={0}, \
			.thrown=0, .stage=0, .line=0}; \
})

# define __ERR_SET(m, ...) do { \
	if (!g__err_ctx) break; \
	g__err_ctx->thrown = 1; \
	g__err_ctx->line = __LINE__; \
	g__err_ctx->file = __FILE__; \
	snprintf(g__err_ctx->msg, ERR_MSG_BUF_SIZE, "Line %d, in %s: "m, __LINE__, __FILE__, ##__VA_ARGS__); \
} while(0)

# define TRY \
    for (t_err_ctx __err=__ERR_INIT; __err.stage < 1; g__err_ctx=__err.prev_ctx) \
		for (; __err.stage < 2; ++__err.stage) \
			if (__builtin_expect(!__err.stage, 1)) \
				do
# define CATCH \
				while (0); \
			else if (__builtin_expect(__err.thrown, 0))

# define THROW_IF(cond, msg, ...) {\
	if (__builtin_expect(!(cond), 0)) {\
		__ERR_SET(msg, ##__VA_ARGS__); \
		break; \
	} \
}
