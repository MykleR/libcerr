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
	CERR_E_ASSERT = 1,
	CERR_E_RUNTIME = 2
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

// DEFAULT TRY STATEMENT
# define TRY                                                                   \
	for (t_err_ctx __err __CERR_CLEANUP=__CERR_INIT, *__p=&__err; __p; __p=0)  \
		if ((__err.thrown=setjmp(__err.frame)) == CERR_E_NONE)

// DEFAULT CATCH STATEMENT
# define CATCH(...)                                                            \
	else if (__CERR_IS_CATCHED(__VA_ARGS__))

// Catches everything else not catched before
# define CATCH_ALL()                                                           \
		else

// Catches exceptions and log the reason
# define CATCH_LOG(...)                                                        \
	CATCH(__VA_ARGS__)                                                         \
	for (char __i=1; __i; __i=0, LOG_ERR("%s", CERR_WHY()))

// Catches everything else and log the reason
# define CATCH_ALL_LOG()                                                       \
	CATCH_ALL()                                                                \
	for (char __i=1; __i; __i=0, LOG_ERR("%s", CERR_WHY()))

// ---- THROW

// DEFAULT THROW
# define THROW(EXCEPTION) do {                                                 \
	if (__CERR_IS_THROWABLE) break;                                            \
	__CERR_SET("");                                                            \
	longjmp(g__err->frame, (EXCEPTION));                                       \
} while (0)

// Throw exception and specify reason
# define THROW_MSG(EXCEPTION, MSG, ...) do {                                   \
	if (__CERR_IS_THROWABLE) break;                                            \
	__CERR_SET(MSG, ##__VA_ARGS__);                                            \
	longjmp(g__err->frame, (EXCEPTION));                                       \
} while (0)

// Throw only if condition is true
# define THROW_IF(EXCEPTION, COND) do {                                        \
	if (__builtin_expect((COND), 0))                                           \
		THROW(EXCEPTION);                                                      \
} while (0)

// Throw only if condition is true and specify reason
# define THROW_IF_MSG(EXCEPTION, COND, MSG, ...) do {                          \
	if (__builtin_expect((COND), 0))                                           \
		THROW_MSG(EXCEPTION, MSG, ##__VA_ARGS__);                              \
} while (0)

// ---- OTHER

// Retreive the reason of the exception as a string of size CERR_MSG_SIZE
#define CERR_WHY() (g__err ? g__err->msg : "")


// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

// helper function for attribute cleanup
static inline void __err_cleanup(t_err_ctx* err) {
	if (err) g__err = err->prev;
}

// Check if exception was thrown
#define __CERR_IS_CATCHED(...) ({                                              \
	CERR_TYPE __errs[] = {__VA_ARGS__};                                        \
	const size_t len = sizeof(__errs)/sizeof(CERR_TYPE);                       \
	int __catched = 0;                                                         \
	for (size_t __i=0; __i < len && !__catched; ++__i)                         \
		__catched = (__err.thrown == __errs[__i]);                             \
	__catched;                                                                 \
})

// Check if exception can be thrown
#define __CERR_IS_THROWABLE                                                    \
	__builtin_expect(!g__err || g__err->thrown != CERR_E_NONE, 0)

// Clear and restore to previous exception context
#define __CERR_CLEANUP                                                         \
	__attribute__((cleanup(__err_cleanup)))

// Init the current exception context
#define __CERR_INIT ({                                                         \
	__err = (t_err_ctx){0};                                                    \
	__err.prev = g__err;                                                       \
	g__err = &__err;                                                           \
	__err;                                                                     \
})

// Define the reason for the current exception context
#define __CERR_SET(MSG, ...)                                                   \
	snprintf(g__err->msg, CERR_MSG_SIZE, "Line %d, in %s: "MSG,                \
		__LINE__, __FILE__, ##__VA_ARGS__)


#ifdef __cplusplus
}
#endif
