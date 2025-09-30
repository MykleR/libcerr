#include "tests.h"

__attribute__((constructor))
void init() { LOG_INFO("LAUNCHING TESTS\n");}

UTEST_MAIN();
