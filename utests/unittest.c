#include <utest.h>
#include <libcerr.h>


__attribute__((constructor))
void init() { LOG_INFO("LAUNCHING TESTS\n");}

UTEST(libcerr, try) {
	UTEST_SKIP("Need to implement this test!");
}

UTEST(libcerr, catch) {
	UTEST_SKIP("Need to implement this test!");
}

UTEST(libcerr, assert) {
	UTEST_SKIP("Need to implement this test!");
}

UTEST(libcerr, throw) {
	UTEST_SKIP("Need to implement this test!");
}

UTEST(libcerr, throw_if) {
	UTEST_SKIP("Need to implement this test!");
}

UTEST_MAIN();
