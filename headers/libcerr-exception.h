#pragma once
#include "libcerr-log.h"
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

typedef enum e_cerr_exception {
	CERR_E_NONE = 0,
	CERR_E_ASSERT = 1
}	t_cerr_exception;

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
		if ((__err.thrown=setjmp(__err.frame)) == CERR_E_NONE)

// ---- CATCH

# define CATCH(EXCEPTION)                                                      \
		else if ((EXCEPTION) != CERR_E_NONE && __err.thrown == (EXCEPTION))

# define CATCH_ALL()                                                           \
		else

# define CATCH_LOG(EXCEPTION)                                                  \
	CATCH(EXCEPTION) do { LOG_ERR("%s", CERR_WHY()); } while (0)

# define CATCH_ALL_LOG()                                                       \
	CATCH_ALL() do { LOG_ERR("%s", CERR_WHY()); } while (0)

// ---- THROW

# define THROW(EXCEPTION) do {                                                 \
	if (__builtin_expect(g__err != NULL, 1)) {                                 \
		__CERR_SET("");                                                        \
		longjmp(g__err->frame, (EXCEPTION));                                   \
	}                                                                          \
} while (0)

# define THROW_MSG(EXCEPTION, MSG, ...) do {                                   \
	if (__builtin_expect(g__err != NULL, 1)) {                                 \
		__CERR_SET(MSG, ##__VA_ARGS__);                                        \
		longjmp(g__err->frame, (EXCEPTION));                                   \
	}                                                                          \
} while (0)

# define THROW_IF(EXCEPTION, COND) do {                                        \
	if (__builtin_expect((COND), 0))                                           \
		THROW(EXCEPTION);                                                      \
} while (0)

# define THROW_IF_MSG(EXCEPTION, COND, MSG, ...) do {                          \
	if (__builtin_expect((COND), 0))                                           \
		THROW_MSG(EXCEPTION, MSG, ##__VA_ARGS__);                              \
} while (0)

// ---- OTHER

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
