#include "libcerr-log.h"
#include <utest.h>
#include <libcerr.h>

# define BAD_TRY_MSG "Reached end of try, FAILURE"
# define BAD_CATCH_MSG "CATCHED A GHOST, FAILURE"
# define GOOD_TRY_MSG "Reached end of try, SUCCESS"
# define GOOD_CATCH_MSG "CATCHED, SUCCESS"


static inline void throw(void){
	THROW(GOOD_CATCH_MSG);
}
static inline void divide(int a, int b){
	THROW_IF(!b, "Division by zero");
}

__attribute__((constructor))
void init() { LOG_INFO("LAUNCHING TESTS\n");}

UTEST(try, simple) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("NOT THROWING...");
		LOG_OK(GOOD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_ERR(BAD_CATCH_MSG);
		ASSERT_TRUE_MSG(0, BAD_CATCH_MSG);
	}
	LOG_NL();
}

UTEST(catch, simple) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING...");
		THROW(GOOD_CATCH_MSG);
		LOG_ERR(BAD_TRY_MSG);
		ASSERT_TRUE_MSG(0, BAD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_OK("%s", ERR_WHY());
	}
	LOG_NL();
}

UTEST(catch, function_call) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING IN FUNCTION CALL...");
		throw();
		LOG_ERR(BAD_TRY_MSG);
		ASSERT_TRUE_MSG(0, BAD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_OK("%s", ERR_WHY());
	}
	LOG_NL();
}

UTEST(catch, single_nested) {
	TRY {
		LOG_DEBUG("Accessing try");
		TRY {
			LOG_DEBUG("Accessing nested try");
			LOG_INFO("THROWING IN NESTED TRY...");
			THROW(GOOD_CATCH_MSG);
			LOG_ERR(BAD_TRY_MSG);
			ASSERT_TRUE_MSG(0, BAD_TRY_MSG);
		} CATCH {
			LOG_DEBUG("Accessing nested catch");
			LOG_OK("nested: %s", ERR_WHY());
		}
		LOG_INFO("NOT THROWING...");
		LOG_OK(GOOD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_OK("%s", BAD_CATCH_MSG);
		ASSERT_TRUE_MSG(0, BAD_CATCH_MSG);
	}
	LOG_NL();
}

UTEST(catch, double_nested) {
	TRY {
		LOG_DEBUG("Accessing try");
		TRY {
			LOG_DEBUG("Accessing nested try");
			LOG_INFO("THROWING IN NESTED TRY...");
			THROW(GOOD_CATCH_MSG);
			LOG_ERR(BAD_TRY_MSG);
			ASSERT_TRUE_MSG(0, BAD_TRY_MSG);
		} CATCH {
			LOG_DEBUG("Accessing nested catch");
			LOG_OK("nested: %s", ERR_WHY());
		}
		LOG_INFO("THROWING...");
		THROW(GOOD_CATCH_MSG);
		LOG_ERR(BAD_TRY_MSG);
		ASSERT_TRUE_MSG(0, BAD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_OK("%s", ERR_WHY());
	}
	LOG_NL();
}

UTEST(catch, throw_if) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING IF, DIVISION BY 0...");
		divide(1, 0);
		LOG_ERR(BAD_TRY_MSG);
		ASSERT_TRUE_MSG(0, BAD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_OK("%s", ERR_WHY());
	}
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING IF, REGULAR DIVISION...");
		divide(1, 2);
		LOG_OK(GOOD_TRY_MSG);
	} CATCH {
		LOG_DEBUG("Accessing catch");
		LOG_OK(BAD_CATCH_MSG);
		ASSERT_TRUE_MSG(0, BAD_CATCH_MSG);
	}
	LOG_NL();
}

UTEST_MAIN();
