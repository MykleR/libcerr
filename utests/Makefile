NAME 				:= tests_cerr

TARGET_PATH			:= ..
TARGET_HEADERS		:= $(TARGET_PATH)/headers
TARGET				:= $(TARGET_PATH)/libcerr.a

TEST_SOURCES_D		:= .
TEST_OBJECTS_D		:= .objs
TEST_LIB_D			:= utest.h

TEST_SOURCES		:= tests_catch.c tests_try.c tests_main.c tests_cache.c
TEST_OBJECTS		:= $(TEST_SOURCES:%.c=$(TEST_OBJECTS_D)/%.o)
TEST_DEPENDENCIES	:= $(TEST_OBJECTS:.o=.d)

CXX					:= gcc
CXXFLAGS			:= -O3 -g -fsanitize=address -fno-omit-frame-pointer
IFLAGS				:= -I $(TARGET_HEADERS) -I $(TEST_LIB_D)

DIR_DUP			= mkdir -p $(@D)

all:
	@$(MAKE) -B test --no-print-directory

-include $(TEST_DEPENDENCIES)

test: $(NAME)
	@./$(NAME) && printf " $(MSG_PASSED)" || (printf " $(MSG_FAILED)")
	@rm -f $(NAME)
	@rm -rf $(TEST_OBJECTS_D)

$(NAME): $(TEST_OBJECTS) $(TARGET)
	@$(CXX) $(CXXFLAGS) $(IFLAGS) $^ -o $@
	@printf " $(MSG_COMPILED)"

$(TARGET):
	@$(MAKE) -B -C $(TARGET_PATH) --no-print-directory

$(TEST_OBJECTS_D)/%.o: %.c
	@$(DIR_DUP)
	@$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -o $@

.PHONY: all test


RED			=	\033[31m
CYAN		=	\033[36m
BOLD		=	\033[1m
ITALIC		=	\033[3m
RESET		=	\033[0m
MSG_COMPILED	= $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiled	$(BOLD)$@$(RESET) $(CYAN)successfully$(RESET)\n
MSG_FAILED	= \n$(RED)$(BOLD)$(ITALIC)■$(RESET) --- $(BOLD)❌ TESTS $(RED)FAILED ❌$(RESET) ---\n
MSG_PASSED	= \n$(CYAN)$(BOLD)$(ITALIC)■$(RESET) --- $(BOLD)✅ ALL TESTS $(CYAN)PASSED ✅$(RESET) ---\n
