#pragma once

# include <stdint.h>
# include <stdlib.h>

# include <libcerr-assert.h>

# ifdef CERR_NCACHE
// No cache

# define MALLOC(S)			malloc(S)
# define CALLOC(N, S)		calloc(N, S)
# define REALLOC(P, S)		realloc(P, S)
# define FREE(P)			free(P)

# else
// ╔═══════════════════════════════[ DEFINITION ]══════════════════════════════╗

# ifndef CERR_CACHE_SIZE
# define CERR_CACHE_SIZE	0x10000
# endif

typedef struct s_cerr_cache {
	void		*allocs[CERR_CACHE_SIZE];
	uint32_t	len;
}	t_cerr_cache;

extern t_cerr_cache g__cerr_cache;

// ╔═════════════════════════════════[ MACROS ]════════════════════════════════╗

# define MALLOC(S) ({                                                          \
	__CACHE_ASSERT_FULL();                                                     \
	void *__res = malloc(S);                                                   \
	ASSERT(__res, __CACHE_FAIL_MSG);                                           \
	__CACHE_INSERT(__res);                                                     \
	__res;                                                                     \
})

# define CALLOC(N, S) ({                                                       \
	__CACHE_ASSERT_FULL();                                                     \
	void *__res = calloc(N, S);                                                \
	ASSERT(__res, __CACHE_FAIL_MSG);                                           \
	__CACHE_INSERT(__res);                                                     \
	__res;                                                                     \
})

# define REALLOC(P, S) ({                                                      \
	void *__prev = P;                                                          \
	void *__res = realloc(P, S);                                               \
	ASSERT(__res, __CACHE_FAIL_MSG);                                           \
	__CACHE_REMOVE(__prev);                                                    \
	__CACHE_INSERT(__res);                                                     \
	__res;                                                                     \
})

# define FREE(P) do {                                                          \
	void *__f = P;                                                             \
	if (__builtin_expect(__f == NULL, 0))                                      \
		break;                                                                 \
	if (__builtin_expect(__CACHE_REMOVE(__f), 1))                              \
		free(__f);                                                             \
	else LOG_WARN(__CACHE_FREE_MSG, __f);                                      \
} while (0)

// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

# define __CACHE_FULL_MSG "libcerr: cache, limit reached, possible memory leak"
# define __CACHE_FAIL_MSG "libcerr: cache, malloc failed, exiting safely..."
# define __CACHE_FREE_MSG "libcerr: cache, double free prevented for pointer %p"
# define __CACHE_WARN_MSG "libcerr: exiting... freed %u possible memory leak"

# define __CACHE_MOD(X) ((X) & (CERR_CACHE_SIZE - 1))

# define __CACHE_ASSERT_FULL()                                                 \
	ASSERT(g__cerr_cache.len < CERR_CACHE_SIZE, __CACHE_FULL_MSG)

# define __CACHE_CLEAR() do {                                                  \
	for (uint32_t i = 0; i < CERR_CACHE_SIZE; ++i)                             \
		free(g__cerr_cache.allocs[i]);                                         \
	if (g__cerr_cache.len)                                                     \
		LOG_WARN(__CACHE_WARN_MSG, g__cerr_cache.len);                         \
	g__cerr_cache = (t_cerr_cache){0};                                         \
} while (0)

# define __CACHE_SEARCH(X, S) ({                                               \
	void	*__x = X;                                                          \
	uint32_t __i = S;                                                          \
	uint32_t __c = 0;                                                          \
	for (;g__cerr_cache.allocs[__i] != __x && __c < CERR_CACHE_SIZE; ++__c)    \
		__i = __CACHE_MOD(__i + 1);                                            \
	__c < CERR_CACHE_SIZE ? __i : UINT32_MAX;                                  \
})

# define __CACHE_INSERT(P) ({                                                  \
	void	*__ptr = P;														   \
	uint32_t __i = __CACHE_SEARCH(NULL, __CACHE_MOD((uintptr_t)__ptr));        \
	g__cerr_cache.allocs[__i] = __ptr;                                         \
	++g__cerr_cache.len;                                                       \
})

# define __CACHE_REMOVE(P) ({                                                  \
	void	*__ptr = P;														   \
	uint32_t __i = __CACHE_SEARCH(__ptr, __CACHE_MOD((uintptr_t)__ptr));       \
	if (__builtin_expect(__i < UINT32_MAX, 1)) {                               \
		g__cerr_cache.allocs[__i] = NULL;                                      \
		--g__cerr_cache.len; }                                                 \
	__i < UINT32_MAX;														   \
})

# ifdef CERR_IMPLEMENTATION
t_cerr_cache g__cerr_cache = {.allocs={0}, .len=0};

__attribute__((destructor))
void __cerr_cache_clear(void) { __CACHE_CLEAR(); }
# endif

# endif
