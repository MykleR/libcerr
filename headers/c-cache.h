#pragma once

# include <c-error.h>
# include <stdint.h>
# include <stdlib.h>

// ╔═══════════════════════════════[ MEM CACHING ]══════════════════════════════╗

# define __CACHE_INIT_SIZE 32
# define __CACHE_FD_MAX 1024

# define __CACHE_FULL_MSG "Cache full, exiting safely..."
# define __CACHE_FAIL_MSG "Malloc failed, exiting safely..."
# define __CACHE_CLR_MSG "Cache: freed %u allocations"

# ifndef NCACHE
// --------------------------------------------------------------
// Dynamic cache

typedef struct s_cache {
	void		**allocs;
	uint32_t	cap;
	uint32_t	len;
}	t_cache;

# define __CACHE_INIT(C) do { \
	if (__builtin_expect((C).allocs == NULL, 0)) { \
		LOG_DEBUG("Initializing cache..."); \
		(C).allocs = calloc((C).cap, sizeof(void *)); \
		ASSERT_MSG((C).allocs, __CACHE_FAIL_MSG); \
}} while (0)

# define __CACHE_CLEAR(C) do { \
	for (uint32_t i = 0; i < (C)->cap; ++i) free((C)->allocs[i]); \
	if ((C)->len) LOG_WARN(__CACHE_CLR_MSG, (C)->len); \
	free((C)->allocs); \
	*(C) = (t_cache){0}; \
} while (0)

# define __CACHE_REALLOC(C) do { \
	ASSERT_MSG(((C)->cap << 1) >> 1 == (C)->cap, __CACHE_FULL_MSG); \
	(C)->cap = (C)->cap << 1; \
	void **new_allocs = calloc((C)->cap, sizeof(void *)); \
	ASSERT_MSG(new_allocs, __CACHE_FAIL_MSG); \
	for (uint32_t i = 0; i < (C)->len; ++i) { \
		uint32_t j = ((uintptr_t)(C)->allocs[i]) & ((C)->cap - 1); \
		for (; new_allocs[j]; j = (j + 1) & ((C)->cap - 1)); \
		new_allocs[j] = (C)->allocs[i]; } \
	free((C)->allocs); \
	(C)->allocs = new_allocs; \
} while (0)

# define __CACHE_INDEX(C, P) ({ \
	uint32_t i = ((uintptr_t)P) & ((C)->cap - 1); \
	uint32_t start = i; \
	int looped = 0; \
	for (; (C)->allocs[i] != P && !looped; i = (i + 1) & ((C)->cap - 1)) \
		looped = (i == start - 1); \
	(looped) ? (uint32_t)(-1) : i; \
})

# define __CACHE_INSERT(C, P) do { \
	uint32_t i = __CACHE_INDEX(C, NULL); \
	ASSERT_MSG(i < UINT32_MAX, __CACHE_FULL_MSG); \
	(C)->allocs[i] = P; \
	++(C)->len; \
} while (0)

# define __CACHE_REMOVE(C, P) do { \
	uint32_t i = __CACHE_INDEX(C, P); \
	if (__builtin_expect(i < UINT32_MAX, 1)) {\
		(C)->allocs[i] = NULL; \
		--(C)->len; \
}} while (0)

# define MALLOC(S) ({ \
	t_cache	*__c = __mem_cache_get(); \
	if (__builtin_expect(__c->len >= __c->cap, 0)) \
		__CACHE_REALLOC(__c); \
	void *ptr = malloc(S); \
	ASSERT_MSG(ptr, __CACHE_FAIL_MSG); \
	__CACHE_INSERT(__c, ptr); \
	ptr; \
})

# define CALLOC(N, S) ({ \
	t_cache	*__c = __mem_cache_get(); \
	if (__builtin_expect(__c->len >= __c->cap, 0)) \
		__CACHE_REALLOC(); \
	void *ptr = calloc(N, S); \
	ASSERT_MSG(ptr, __CACHE_FAIL_MSG); \
	__CACHE_INSERT(__c, ptr); \
	ptr; \
})

# define REALLOC(P, S) ({ \
	t_cache	*__c = __mem_cache_get(); \
	void *ptr = realloc(P, S); \
	ASSERT_MSG(ptr != NULL, __CACHE_FAIL_MSG); \
	__CACHE_REMOVE(__c, P); \
	__CACHE_INSERT(__c, ptr); \
	ptr; \
})

# define FREE(P) do { \
	t_cache	*__c = __mem_cache_get(); \
	__CACHE_REMOVE(__c, P); \
	free(P); \
	P = NULL; \
} while (0)

t_cache *__mem_cache_get(void) {
	static t_cache __c = {.allocs=NULL, .cap=__CACHE_INIT_SIZE, .len=0};
		__CACHE_INIT(__c);
	return &(__c);
}

__attribute__((destructor))
void __mem_cache_clear(void) {
	t_cache *__c = __mem_cache_get();
	__CACHE_CLEAR(__c);
}

# else
// --------------------------------------------------------------
// No cache

# define MALLOC(S)			malloc(S)
# define CALLOC(N, S)		calloc(N, S)
# define REALLOC(P, S)		realloc(P, S)
# define FREE(P)			free(P)

# endif
