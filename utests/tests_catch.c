#include "tests.h"

static inline void throw(void) {
	THROW_MSG(ERROR, GOOD_CATCH_MSG);
}
static inline void divide(int UNUSED a, int b) {
	THROW_IF_MSG(ERROR, !b, "Division by zero");
}

UTEST(catch, simple) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING...");
		THROW_MSG(ERROR, GOOD_CATCH_MSG);
		END_BAD_TEST(BAD_TRY_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_GOOD_TEST(CERR_WHY());
	}
	END_BAD_TEST(BAD_CATCH_END_MSG);
}

UTEST(catch, codes) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING...");
		THROW_MSG(ERROR, GOOD_CATCH_MSG);
		END_BAD_TEST(BAD_TRY_MSG);
	} CATCH(!ERROR) {
		LOG_DEBUG("Accessing catch code %d", ERROR);
		END_BAD_TEST(BAD_CATCH_MSG);
	} CATCH(ERROR + 1) {
		LOG_DEBUG("Accessing catch code %d", ERROR + 1);
		END_BAD_TEST(BAD_CATCH_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch code %d", ERROR);
		END_GOOD_TEST(CERR_WHY());
	}
	END_BAD_TEST(BAD_CATCH_END_MSG);
}

UTEST(catch, function_call) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING IN FUNCTION CALL...");
		throw();
		END_BAD_TEST(BAD_TRY_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_GOOD_TEST(CERR_WHY());
	}
	END_BAD_TEST(BAD_CATCH_END_MSG);
}

UTEST(catch, single_nested) {
	TRY {
		LOG_DEBUG("Accessing try");
		TRY {
			LOG_DEBUG("Accessing nested try");
			LOG_INFO("THROWING IN NESTED TRY...");
			THROW_MSG(ERROR, GOOD_CATCH_MSG);
			END_BAD_TEST(BAD_TRY_MSG);
		} CATCH(ERROR) {
			LOG_DEBUG("Accessing nested catch");
			LOG_OK("nested: %s", CERR_WHY());
		}
		LOG_INFO("NOT THROWING...");
		END_GOOD_TEST(GOOD_TRY_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_BAD_TEST(BAD_CATCH_MSG);
	}
	END_BAD_TEST(BAD_TRY_END_MSG);
}

UTEST(catch, double_nested) {
	TRY {
		LOG_DEBUG("Accessing try");
		TRY {
			LOG_DEBUG("Accessing nested try");
			LOG_INFO("THROWING IN NESTED TRY...");
			THROW_MSG(ERROR, GOOD_CATCH_MSG);
			END_BAD_TEST(BAD_TRY_MSG);
		} CATCH(ERROR) {
			LOG_DEBUG("Accessing nested catch");
			LOG_OK("nested: %s", CERR_WHY());
		}
		LOG_INFO("THROWING...");
		THROW_MSG(ERROR, GOOD_CATCH_MSG);
		END_BAD_TEST(BAD_TRY_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_GOOD_TEST(CERR_WHY());
	}
	END_BAD_TEST(BAD_CATCH_END_MSG);
}

UTEST(catch, throw_if) {
	TRY {
		LOG_DEBUG("Accessing try");
		LOG_INFO("THROWING IF, NO DIVISION BY 0...");
		divide(1, 2);
		LOG_OK("No errors thrown, SUCCESS");
		LOG_INFO("THROWING IF, DIVISION BY 0...");
		divide(1, 0);
		END_BAD_TEST(BAD_TRY_MSG);
	} CATCH(ERROR) {
		LOG_DEBUG("Accessing catch");
		END_GOOD_TEST(CERR_WHY());
	}
	END_BAD_TEST(BAD_CATCH_END_MSG);
}
