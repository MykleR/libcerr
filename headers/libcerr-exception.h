#pragma once

#include <stdlib.h>
#include <setjmp.h>
#include <stdint.h>

#include <libcerr-log.h>
#include <libcerr-assert.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #define CERR_TLS _Thread_local
#else
  #define CERR_TLS __thread
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ╔═══════════════════════════════[ DEFINITION ]══════════════════════════════╗

#define		CERR_E_NONE		0
#define		CERR_E_RUNTIME	1

#ifndef		CERR_MSG_SIZE
# define	CERR_MSG_SIZE 1024
#endif
#ifndef		CERR_TYPE
# define	CERR_TYPE uint_fast32_t
#endif


typedef struct s_err_ctx t_err_ctx;
struct s_err_ctx {
	t_err_ctx	*prev;
	jmp_buf		frame;
	CERR_TYPE	thrown;
	char		msg[CERR_MSG_SIZE];
};

extern CERR_TLS t_err_ctx *g__cerr_ctx;

#ifdef CERR_IMPLEMENTATION
CERR_TLS t_err_ctx *g__cerr_ctx = NULL;
#endif

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
	__CERR_IS_THROWABLE(EXCEPTION);                                            \
	__CERR_SET("");                                                            \
	longjmp(g__cerr_ctx->frame, (EXCEPTION));                                  \
} while (0)

// Throw exception and specify reason
# define THROW_MSG(EXCEPTION, MSG, ...) do {                                   \
	__CERR_IS_THROWABLE(EXCEPTION);\
	__CERR_SET(MSG, ##__VA_ARGS__);                                            \
	longjmp(g__cerr_ctx->frame, (EXCEPTION));                                  \
} while (0)

// Throw only if condition is true
# define THROW_IF(COND, EXCEPTION) do {                                        \
	if (__builtin_expect((COND), 0))                                           \
		THROW(EXCEPTION);                                                      \
} while (0)

// Throw only if condition is true and specify reason
# define THROW_IF_MSG(COND, EXCEPTION, MSG, ...) do {                          \
	if (__builtin_expect((COND), 0))                                           \
		THROW_MSG(EXCEPTION, MSG, ##__VA_ARGS__);                              \
} while (0)

// ---- OTHER

// Retreive the reason of the exception as a string of size CERR_MSG_SIZE
#define CERR_WHY() (g__cerr_ctx ? g__cerr_ctx->msg : "")


// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

// helper function for attribute cleanup
static inline void __err_cleanup(t_err_ctx* err) {
	if (err) g__cerr_ctx = err->prev;
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
#define __CERR_IS_THROWABLE(EXCEPTION)                                         \
	ASSERT(g__cerr_ctx != NULL && g__cerr_ctx->thrown == CERR_E_NONE,          \
		"Thrown exception[%d] not caught.", (EXCEPTION))

// Clear and restore to previous exception context
#define __CERR_CLEANUP                                                         \
	__attribute__((cleanup(__err_cleanup)))

// Init the current exception context
#define __CERR_INIT ({                                                         \
	__err = (t_err_ctx){0};                                                    \
	__err.prev = g__cerr_ctx;                                                  \
	g__cerr_ctx = &__err;                                                      \
	__err;                                                                     \
})

// Define the reason for the current exception context
#define __CERR_SET(MSG, ...)                                                   \
	snprintf(g__cerr_ctx->msg, CERR_MSG_SIZE, "Line %d, in %s: "MSG,           \
		__LINE__, __FILE__, ##__VA_ARGS__)


#ifdef __cplusplus
}
#endif
