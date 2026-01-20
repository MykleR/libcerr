#include "tests.h"
#include <string.h>

// ═══════════════════════════════[ MALLOC TESTS ]═══════════════════════════════

UTEST(cache_malloc, simple) {
	void *ptr = MALLOC(100);
	ASSERT_TRUE_MSG(ptr != NULL, "MALLOC returned NULL");
	ASSERT_EQ(g__cerr_cache.len, 1);
	FREE(ptr);
	ASSERT_EQ(g__cerr_cache.len, 0);
}

UTEST(cache_malloc, multiple) {
	static const int N = 100;
	void *ptrs[N];
	
	for (int i = 0; i < N; ++i) {
		ptrs[i] = MALLOC(16);
		ASSERT_TRUE_MSG(ptrs[i] != NULL, "MALLOC returned NULL");
	}
	ASSERT_EQ(g__cerr_cache.len, N);
	
	for (int i = 0; i < N; ++i) {
		FREE(ptrs[i]);
	}
	ASSERT_EQ(g__cerr_cache. len, 0);
}

UTEST(cache_malloc, write_read) {
	char *ptr = MALLOC(256);
	ASSERT_TRUE_MSG(ptr != NULL, "MALLOC returned NULL");
	memset(ptr, 'A', 255);
	ptr[255] = '\0';
	ASSERT_EQ(strlen(ptr), 255);
	FREE(ptr);
}

// ═══════════════════════════════[ CALLOC TESTS ]═══════════��═══════════════════

UTEST(cache_calloc, simple) {
	int *ptr = CALLOC(10, sizeof(int));
	ASSERT_TRUE_MSG(ptr != NULL, "CALLOC returned NULL");
	ASSERT_EQ(g__cerr_cache. len, 1);
	
	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(ptr[i], 0);
	}
	FREE(ptr);
	ASSERT_EQ(g__cerr_cache.len, 0);
}

UTEST(cache_calloc, multiple) {
	static const int N = 50;
	void *ptrs[N];
	
	for (int i = 0; i < N; ++i) {
		ptrs[i] = CALLOC(1, 32);
		ASSERT_TRUE_MSG(ptrs[i] != NULL, "CALLOC returned NULL");
	}
	ASSERT_EQ(g__cerr_cache.len, N);
	
	for (int i = 0; i < N; ++i) {
		FREE(ptrs[i]);
	}
	ASSERT_EQ(g__cerr_cache.len, 0);
}

// ═══════════════════════════════[ REALLOC TESTS ]══════════════════════════════

UTEST(cache_realloc, simple_grow) {
	char *ptr = MALLOC(16);
	ASSERT_TRUE_MSG(ptr != NULL, "MALLOC returned NULL");
	strcpy(ptr, "Hello");
	
	ptr = REALLOC(ptr, 64);
	ASSERT_TRUE_MSG(ptr != NULL, "REALLOC returned NULL");
	ASSERT_STREQ(ptr, "Hello");
	ASSERT_EQ(g__cerr_cache.len, 1);
	
	FREE(ptr);
	ASSERT_EQ(g__cerr_cache.len, 0);
}

UTEST(cache_realloc, simple_shrink) {
	char *ptr = MALLOC(256);
	ASSERT_TRUE_MSG(ptr != NULL, "MALLOC returned NULL");
	strcpy(ptr, "Test");
	
	ptr = REALLOC(ptr, 16);
	ASSERT_TRUE_MSG(ptr != NULL, "REALLOC returned NULL");
	ASSERT_STREQ(ptr, "Test");
	ASSERT_EQ(g__cerr_cache.len, 1);
	
	FREE(ptr);
}

UTEST(cache_realloc, from_null) {
	char *ptr = REALLOC(NULL, 32);
	ASSERT_TRUE_MSG(ptr != NULL, "REALLOC(NULL, size) returned NULL");
	ASSERT_EQ(g__cerr_cache.len, 1);
	
	FREE(ptr);
	ASSERT_EQ(g__cerr_cache.len, 0);
}

UTEST(cache_realloc, multiple_grows) {
	char *ptr = MALLOC(8);
	ASSERT_TRUE_MSG(ptr != NULL, "MALLOC returned NULL");
	
	for (int i = 0; i < 5; ++i) {
		ptr = REALLOC(ptr, 8 << (i + 1));
		ASSERT_TRUE_MSG(ptr != NULL, "REALLOC returned NULL");
		ASSERT_EQ(g__cerr_cache.len, 1);
	}
	
	FREE(ptr);
	ASSERT_EQ(g__cerr_cache.len, 0);
}

// ═══════════════════════════════[ FREE TESTS ]═════════════════════════════════

UTEST(cache_free, null_pointer) {
	uint32_t initial_len = g__cerr_cache.len;
	FREE(NULL);
	ASSERT_EQ(g__cerr_cache.len, initial_len);
}

UTEST(cache_free, double_free_protection) {
	char *ptr = MALLOC(32);
	ASSERT_TRUE_MSG(ptr != NULL, "MALLOC returned NULL");
	
	FREE(ptr);
	ASSERT_EQ(g__cerr_cache.len, 0);
	
	LOG_INFO("Expected warning about untracked pointer:");
	FREE(ptr); // Should warn but not crash
	ASSERT_EQ(g__cerr_cache.len, 0);
}

UTEST(cache_free, out_of_order) {
	void *a = MALLOC(16);
	void *b = MALLOC(16);
	void *c = MALLOC(16);
	ASSERT_EQ(g__cerr_cache.len, 3);
	
	FREE(b);
	ASSERT_EQ(g__cerr_cache.len, 2);
	FREE(a);
	ASSERT_EQ(g__cerr_cache.len, 1);
	FREE(c);
	ASSERT_EQ(g__cerr_cache. len, 0);
}

// ═══════════════════════════════[ CACHE INTEGRITY TESTS ]══════════════════════

UTEST(cache_integrity, pointer_in_cache) {
	void *a = MALLOC(16);
	void *b = MALLOC(16);
	void *c = MALLOC(16);
	
	// Verify each pointer actually exists in cache array
	uint32_t idx_a = __CERR_CACHE_SEARCH(a, __CERR_MOD((uintptr_t)a));
	uint32_t idx_b = __CERR_CACHE_SEARCH(b, __CERR_MOD((uintptr_t)b));
	uint32_t idx_c = __CERR_CACHE_SEARCH(c, __CERR_MOD((uintptr_t)c));
	
	ASSERT_TRUE(idx_a < CERR_CACHE_SIZE);
	ASSERT_TRUE(idx_b < CERR_CACHE_SIZE);
	ASSERT_TRUE(idx_c < CERR_CACHE_SIZE);
	
	ASSERT_EQ(g__cerr_cache.allocs[idx_a], a);
	ASSERT_EQ(g__cerr_cache.allocs[idx_b], b);
	ASSERT_EQ(g__cerr_cache.allocs[idx_c], c);
	
	FREE(a);
	FREE(b);
	FREE(c);
}

UTEST(cache_integrity, pointer_removed_after_free) {
	void *ptr = MALLOC(32);
	
	uint32_t idx_before = __CERR_CACHE_SEARCH(ptr, __CERR_MOD((uintptr_t)ptr));
	ASSERT_TRUE(idx_before < CERR_CACHE_SIZE);
	
	FREE(ptr);
	
	uint32_t idx_after = __CERR_CACHE_SEARCH(ptr, __CERR_MOD((uintptr_t)ptr));
	ASSERT_TRUE(idx_after >= CERR_CACHE_SIZE); // Not found
}

UTEST(cache_integrity, realloc_updates_cache) {
	void *old_ptr = MALLOC(16);
	
	uint32_t idx_old = __CERR_CACHE_SEARCH(old_ptr, __CERR_MOD((uintptr_t)old_ptr));
	ASSERT_TRUE(idx_old < CERR_CACHE_SIZE);
	
	void *new_ptr = REALLOC(old_ptr, 256);
	
	// New pointer must be in cache
	uint32_t idx_new = __CERR_CACHE_SEARCH(new_ptr, __CERR_MOD((uintptr_t)new_ptr));
	ASSERT_TRUE(idx_new < CERR_CACHE_SIZE);
	
	// If address changed, old pointer must be gone
	if (old_ptr != new_ptr) {
		uint32_t idx_old_after = __CERR_CACHE_SEARCH(old_ptr, __CERR_MOD((uintptr_t)old_ptr));
		ASSERT_TRUE(idx_old_after >= CERR_CACHE_SIZE);
	}
	
	FREE(new_ptr);
}

// ═══════════════════════════════[ HASH COLLISION TESTS ]═══════════════════════

UTEST(cache_hash, collision_handling) {
	static const int N = 200;
	void *ptrs[N];
	
	for (int i = 0; i < N; ++i) {
		ptrs[i] = MALLOC(1);
		ASSERT_TRUE_MSG(ptrs[i] != NULL, "MALLOC returned NULL");
		
		// Verify it's findable immediately after insert
		uint32_t idx = __CERR_CACHE_SEARCH(ptrs[i], __CERR_MOD((uintptr_t)ptrs[i]));
		ASSERT_TRUE(idx < CERR_CACHE_SIZE);
	}
	
	ASSERT_EQ(g__cerr_cache.len, N);
	
	for (int i = 0; i < N; ++i) {
		FREE(ptrs[i]);
	}
	
	ASSERT_EQ(g__cerr_cache.len, 0);
}

// ═══════════════════════════════[ MIXED TESTS ]════════════════════════════════

UTEST(cache_mixed, stress_test) {
	static const int N = 500;
	void *ptrs[N];
	
	for (int i = 0; i < N; ++i) {
		if (i % 3 == 0) {
			ptrs[i] = MALLOC(16 + (i % 64));
		} else if (i % 3 == 1) {
			ptrs[i] = CALLOC(1, 16 + (i % 64));
		} else {
			ptrs[i] = MALLOC(8);
			ptrs[i] = REALLOC(ptrs[i], 16 + (i % 64));
		}
		ASSERT_TRUE_MSG(ptrs[i] != NULL, "Allocation failed");
	}
	ASSERT_EQ(g__cerr_cache.len, N);
	
	// Free in reverse-ish order
	for (int i = N - 1; i >= 0; i -= 2) {
		FREE(ptrs[i]);
	}
	for (int i = 0; i < N; i += 2) {
		FREE(ptrs[i]);
	}
	ASSERT_EQ(g__cerr_cache. len, 0);
}

// ═══════════════════════════════[ DESTRUCTOR TEST ]════════════════════════════

UTEST(cache_clear, clears_all) {
	void *a = MALLOC(16);
	void *b = CALLOC(4, 8);
	void *c = MALLOC(32);
	(void)a; (void)b; (void)c;
	ASSERT_EQ(g__cerr_cache.len, 3);
	
	LOG_INFO("Testing __CERR_CACHE_CLEAR (expect leak warning):");
	__CERR_CACHE_CLEAR();
	ASSERT_EQ(g__cerr_cache.len, 0);
}
