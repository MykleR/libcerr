#include "libcerr-log.h"
#include "tests.h"
#include <time.h>

static clock_t g_begin;

__attribute__((constructor))
void init() {
	LOG_INFO("LAUNCHING TESTS\n");
	g_begin = clock();
}

__attribute__((destructor))
void end() {
	double time_spent = (double)(clock() - g_begin) / CLOCKS_PER_SEC;
	LOG_NL();
	LOG_INFO("TESTS ENDED AFTER %lf seconds\n", time_spent);
}

UTEST_MAIN();
