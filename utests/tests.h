#pragma once

# include <utest.h>
# include <libcerr.h>

# define UNUSED __attribute__((unused))

# define ERROR 1

# define BAD_TRY_MSG		"Reached end of try, FAILURE"
# define BAD_TRY_END_MSG	"TRY BODY NOT REACHED OR RETURNED"
# define BAD_CATCH_MSG		"CATCHED A GHOST, FAILURE"
# define BAD_CATCH_END_MSG	"CATCH BODY NOT REACHED OR RETURNED"
# define GOOD_TRY_MSG		"Reached end of try, SUCCESS"
# define GOOD_CATCH_MSG		"CATCHED, SUCCESS"

# define END_BAD_TEST(M) \
	LOG_ERR("%s", M); \
	ASSERT_TRUE_MSG(0, M)

# define END_GOOD_TEST(M) \
	LOG_OK("%s", M); \
	LOG_NL(); \
	return 
