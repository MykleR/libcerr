#pragma once
#include <stdlib.h>
#include <setjmp.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #define CERR_TLS _Thread_local
#else
  #define CERR_TLS __thread
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ╔═══════════════════════════════[ DEFINITION ]══════════════════════════════╗

#ifndef CERR_MSG_SIZE
# define CERR_MSG_SIZE		1024
#endif
#ifndef CERR_TYPE
# define CERR_TYPE			unsigned int
#endif
#define CERR_NOEXCEPTION	0x0

typedef struct s_err_ctx t_err_ctx;
struct s_err_ctx {
	t_err_ctx	*prev;
	jmp_buf		frame;
	CERR_TYPE	thrown;
	char		msg[CERR_MSG_SIZE];
};

static CERR_TLS t_err_ctx *g__err = NULL;


// ╔═════════════════════════════════[ MACROS ]════════════════════════════════╗

# define TRY                                                                   \
	for (t_err_ctx __err __CERR_CLEANUP=__CERR_INIT, *__p=&__err; __p; __p=0)  \
		if ((__err.thrown=setjmp(__err.frame)) == CERR_NOEXCEPTION)

# define CATCH(CODE)                                                           \
		else if ((CODE) != CERR_NOEXCEPTION && __err.thrown == (CODE))

# define THROW(CODE) do {                                                      \
	if (__builtin_expect(g__err != NULL, 1)) {                                 \
		__CERR_SET("");                                                        \
		longjmp(g__err->frame, (CODE));                                        \
	}                                                                          \
} while (0)

# define THROW_MSG(CODE, MSG, ...) do {                                        \
	if (__builtin_expect(g__err != NULL, 1)) {                                 \
		__CERR_SET(MSG, ##__VA_ARGS__);                                        \
		longjmp(g__err->frame, (CODE));                                        \
	}                                                                          \
} while (0)

# define THROW_IF(CODE, COND) do {                                             \
	if (__builtin_expect((COND), 0))                                           \
		THROW(CODE);                                                           \
} while (0)

# define THROW_IF_MSG(CODE, COND, MSG, ...) do {                               \
	if (__builtin_expect((COND), 0))                                           \
		THROW_MSG(CODE, MSG, ##__VA_ARGS__);                                   \
} while (0)

#define CERR_WHY() (g__err ? g__err->msg : "")

// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

static inline void __err_cleanup(t_err_ctx* err) {
	if (err) g__err = err->prev;
}

#define __CERR_CLEANUP                                                         \
	__attribute__((cleanup(__err_cleanup)))

#define __CERR_INIT ({                                                         \
	__err = (t_err_ctx){0};                                                    \
	__err.prev = g__err;                                                       \
	g__err = &__err;                                                           \
	__err;                                                                     \
})

#define __CERR_SET(MSG, ...)                                                   \
	snprintf(g__err->msg, CERR_MSG_SIZE, "Line %d, in %s: "MSG,                \
		__LINE__, __FILE__, ##__VA_ARGS__)


#ifdef __cplusplus
}
#endif
