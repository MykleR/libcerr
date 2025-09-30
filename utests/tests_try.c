#include "tests.h"

UTEST(try, no_return) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("NOT THROWING...");
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_BAD_TEST(BAD_CATCH_MSG);
	}
	END_GOOD_TEST(GOOD_TRY_MSG);
}

UTEST(try, simple) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("NOT THROWING...");
		END_GOOD_TEST(GOOD_TRY_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_BAD_TEST(BAD_CATCH_MSG);
	}
	END_BAD_TEST(BAD_TRY_END_MSG);
}

UTEST(try, loop) {
	static const int N=1000;
	volatile int count = 0;
	int i = N;
	while (i--) {
		TRY {
			THROW_MSG(ERROR, GOOD_CATCH_MSG);
		} CATCH(ERROR) {count++;}
	}
	ASSERT_EQ(count, N);
}
