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

// IMPORTANT: CERR_CACHE_SIZE MUST BE A POWER OF TWO
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
	ASSERT(g__cerr_cache.len < CERR_CACHE_SIZE, __CERR_M_FULL)                 \
	void *__res = malloc(S);                                                   \
	ASSERT(__res, __CERR_M_AFAIL);                                             \
	__CERR_CACHE_INSERT(__res);                                                \
	__res;                                                                     \
})

# define CALLOC(N, S) ({                                                       \
	ASSERT(g__cerr_cache.len < CERR_CACHE_SIZE, __CERR_M_FULL)                 \
	void *__res = calloc(N, S);                                                \
	ASSERT(__res, __CERR_M_AFAIL);                                             \
	__CERR_CACHE_INSERT(__res);                                                \
	__res;                                                                     \
})

# define REALLOC(P, S) ({                                                      \
	void	*__res = NULL;                                                     \
	void	*__prev = P;                                                       \
	uint32_t __rm = !__prev || __CERR_CACHE_REMOVE(__prev);                    \
	ASSERT(__rm, __CERR_M_RFAIL, __prev);                                      \
	__res = realloc(__prev, S);                                                \
	ASSERT(__res, __CERR_M_AFAIL);                                             \
	__CERR_CACHE_INSERT(__res);                                                \
	__res;                                                                     \
})

# define FREE(P) do {                                                          \
	void	*__f = P;                                                          \
	uint32_t __rm = 0;                                                         \
	if (__builtin_expect(!__f, 0)) break;                                      \
	__rm = __CERR_CACHE_REMOVE(__f);                                           \
	if (__builtin_expect(__rm, 1))                                             \
		free(__f);                                                             \
	else LOG_WARN(__CERR_M_FFAIL, __f);                                        \
} while (0)

// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

# define __CERR_M_FULL "libcerr: cache, limit reached, possible memory leak."
# define __CERR_M_AFAIL "libcerr: cache, alloc failed, exiting safely."
# define __CERR_M_FFAIL "libcerr: cache, ignoring free on untracked pointer %p"
# define __CERR_M_RFAIL "libcerr: cache, realloc on untracked pointer %p"
# define __CERR_M_WEXIT "libcerr: cache exit, freed %u possible memory leak."

# define __CERR_MOD(X) ((X) & (CERR_CACHE_SIZE - 1))

# define __CERR_CACHE_CLEAR() do {                                             \
	for (uint32_t i = 0; i < CERR_CACHE_SIZE; ++i)                             \
		free(g__cerr_cache.allocs[i]);                                         \
	if (g__cerr_cache.len)                                                     \
		LOG_WARN(__CERR_M_WEXIT, g__cerr_cache.len);                           \
	g__cerr_cache = (t_cerr_cache){0};                                         \
} while (0)

# define __CERR_CACHE_SEARCH(X, START) ({                                      \
	void	*__x = X;                                                          \
	uint32_t __i = START;                                                      \
	uint32_t __c = 0;                                                          \
	for (;g__cerr_cache.allocs[__i] != __x && __c < CERR_CACHE_SIZE; ++__c)    \
		__i = __CERR_MOD(__i + 1);                                             \
	__c < CERR_CACHE_SIZE ? __i : __c;                                         \
})

# define __CERR_CACHE_INSERT(P) ({                                             \
	void	*__ptr = P;                                                        \
	uint32_t __i = __CERR_CACHE_SEARCH(NULL, __CERR_MOD((uintptr_t)__ptr));    \
	g__cerr_cache.allocs[__i] = __ptr;                                         \
	++g__cerr_cache.len;                                                       \
})

# define __CERR_CACHE_REMOVE(P) ({                                             \
	void	*__ptr = P;                                                        \
	uint32_t __i = __CERR_CACHE_SEARCH(__ptr, __CERR_MOD((uintptr_t)__ptr));   \
	uint32_t __bool = __i < CERR_CACHE_SIZE;                                   \
	if (__builtin_expect(__bool, 1)) {                                         \
		g__cerr_cache.allocs[__i] = NULL;                                      \
		--g__cerr_cache.len;                                                   \
	} __bool;                                                                  \
})

# ifdef CERR_IMPLEMENTATION
t_cerr_cache g__cerr_cache = {.allocs={0}, .len=0};

__attribute__((destructor))
void __cerr_cache_clear(void) { __CERR_CACHE_CLEAR(); }
# endif

# endif
