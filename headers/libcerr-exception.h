#pragma once

# include <stdint.h>
# include <stdlib.h>
# include <setjmp.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #define ERR_TLS _Thread_local
#else
  #define ERR_TLS __thread
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ╔═══════════════════════════════[ DEFINITION ]══════════════════════════════╗

# ifndef ERR_MSG_SIZE
#  define ERR_MSG_SIZE 1024
# endif

typedef struct s_err_ctx {
	struct s_err_ctx	*prev_ctx;
	const char			*file;
	jmp_buf				env;
	char				msg[ERR_MSG_SIZE];
	int					line:8;
	int					thrown:8;
} t_err_ctx;

static ERR_TLS t_err_ctx *g__err = NULL;


// ╔═════════════════════════════════[ MACROS ]════════════════════════════════╗


# define TRY \
    for (t_err_ctx __err=__ERR_INIT, *__p=&__err; __p; __p=0,g__err=__err.prev_ctx) \
		if (setjmp(__err.env) == 0) do

# define CATCH \
	while (0); \
	else for (;!__err.thrown; __err.thrown=1)

# define THROW(msg, ...) do { \
	if (__builtin_expect(g__err != NULL, 1)) { \
		__ERR_SET(msg, ##__VA_ARGS__) \
		longjmp(g__err->env, 1); \
	} \
} while (0)

# define THROW_IF(cond, msg, ...) do {\
	if (__builtin_expect((cond), 0)) \
		THROW(msg, ##__VA_ARGS__); \
} while (0)

# define ERR_WHY() (g__err ? g__err->msg : "")
# define ERR_FILE() (g__err ? g__err->file : "")
# define ERR_LINE() (g__err ? g__err->line : 0)

// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

# define __ERR_INIT ({ \
	t_err_ctx *prev = g__err; \
	g__err = &__err ; \
	(t_err_ctx){.prev_ctx=prev, .file=0, .env={0}, .msg={0}, .line=0}; \
})

# define __ERR_SET(m, ...) \
	g__err->file = __FILE__; \
	g__err->line = __LINE__; \
	snprintf(g__err->msg, ERR_MSG_SIZE, "Line %d, in %s: "m, \
		__LINE__, __FILE__, ##__VA_ARGS__);

#ifdef __cplusplus
}
#endif
