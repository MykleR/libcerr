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

# define __CACHE_INIT_SIZE 2
# define __CACHE_FD_MAX 1024

# define __CACHE_FULL_MSG "libcerr-cache full, exiting safely..."
# define __CACHE_FAIL_MSG "libcerr-cache malloc failed, exiting safely..."
# define __CACHE_WARN_MSG "libcerr-cache tracked possible memory leak of %u allocations"
# define __CACHE_FATAL_MSG "libcerr-cache not Initialized."

typedef struct s_cerr_cache {
	void		**allocs;
	uint32_t	cap;
	uint32_t	len;
}	t_cerr_cache;

extern t_cerr_cache g__cerr_cache;

// ╔═════════════════════════════════[ MACROS ]════════════════════════════════╗

# define MALLOC(S) ({ \
	__CACHE_ASSERT_INIT(g__cerr_cache); \
	if (__builtin_expect(g__cerr_cache.len >= g__cerr_cache.cap, 0)) \
		__CACHE_REALLOC(g__cerr_cache); \
	void *ptr = malloc(S); \
	ASSERT(ptr, __CACHE_FAIL_MSG); \
	__CACHE_INSERT(g__cerr_cache, ptr); \
	ptr; \
})

# define CALLOC(N, S) ({ \
	__CACHE_ASSERT_INIT(g__cerr_cache); \
	if (__builtin_expect(g__cerr_cache.len >= g__cerr_cache.cap, 0)) \
		__CACHE_REALLOC(); \
	void *ptr = calloc(N, S); \
	ASSERT(ptr, __CACHE_FAIL_MSG); \
	__CACHE_INSERT(g__cerr_cache, ptr); \
	ptr; \
})

# define REALLOC(P, S) ({ \
	__CACHE_ASSERT_INIT(g__cerr_cache); \
	void *ptr = realloc(P, S); \
	ASSERT(ptr != NULL, __CACHE_FAIL_MSG); \
	__CACHE_REMOVE(g__cerr_cache, P); \
	__CACHE_INSERT(g__cerr_cache, ptr); \
	ptr; \
})

# define FREE(P) do { \
	__CACHE_ASSERT_INIT(g__cerr_cache); \
	__CACHE_REMOVE(g__cerr_cache, P); \
	free(P); \
	P = NULL; \
} while (0)

// ╔══════════════════════════════════[ UTILS ]════════════════════════════════╗

# define __CACHE_ASSERT_INIT(C) \
	ASSERT((C).allocs != NULL, __CACHE_FATAL_MSG);

# define __CACHE_INIT(C) do { \
	LOG_DEBUG("Initializing cache..."); \
	(C).allocs = calloc((C).cap, sizeof(void *)); \
	ASSERT((C).allocs, __CACHE_FAIL_MSG); \
} while (0)

# define __CACHE_CLEAR(C) do { \
	LOG_DEBUG("Clearing cache..."); \
	for (uint32_t i = 0; i < (C).cap; ++i) free((C).allocs[i]); \
	if ((C).len) LOG_WARN(__CACHE_WARN_MSG, (C).len); \
	free((C).allocs); \
	(C) = (t_cerr_cache){0}; \
} while (0)

# define __CACHE_REALLOC(C) do { \
	ASSERT(((C).cap << 1) >> 1 == (C).cap, __CACHE_FULL_MSG); \
	(C).cap = (C).cap << 1; \
	void **new_allocs = calloc((C).cap, sizeof(void *)); \
	ASSERT(new_allocs, __CACHE_FAIL_MSG); \
	for (uint32_t i = 0; i < (C).len; ++i) { \
		uint32_t j = ((uintptr_t)(C).allocs[i]) & ((C).cap - 1); \
		for (; new_allocs[j]; j = (j + 1) & ((C).cap - 1)); \
		new_allocs[j] = (C).allocs[i]; } \
	free((C).allocs); \
	(C).allocs = new_allocs; \
} while (0)

# define __CACHE_INDEX(C, P) ({ \
	uint32_t i = ((uintptr_t)P) & ((C).cap - 1); \
	uint32_t start = i; \
	int looped = 0; \
	for (; (C).allocs[i] != P && !looped; i = (i + 1) & ((C).cap - 1)) \
		looped = (i == start - 1); \
	(looped) ? (uint32_t)(-1) : i; \
})

# define __CACHE_INSERT(C, P) do { \
	uint32_t i = __CACHE_INDEX(C, NULL); \
	ASSERT(i < UINT32_MAX, __CACHE_FULL_MSG); \
	(C).allocs[i] = P; \
	++(C).len; \
} while (0)

# define __CACHE_REMOVE(C, P) do { \
	uint32_t i = __CACHE_INDEX(C, P); \
	if (__builtin_expect(i < UINT32_MAX, 1)) {\
		(C).allocs[i] = NULL; \
		--(C).len; \
}} while (0)

# ifdef CERR_IMPLEMENTATION
t_cerr_cache g__cerr_cache = {.allocs=NULL, .cap=__CACHE_INIT_SIZE, .len=0 };

__attribute__((constructor))
void __cerr_cache_init(void) { __CACHE_INIT(g__cerr_cache); }

__attribute__((destructor))
void __cerr_cache_clear(void) { __CACHE_CLEAR(g__cerr_cache); }
# endif
# endif
