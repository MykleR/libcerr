NAME 				:= tests

TARGET_PATH			:= ../
TARGET_HEADERS		:= $(TARGET_PATH)/headers
TARGET				:= $(TARGET_PATH)/libcerr.a

TEST_SOURCES_D		:= .
TEST_OBJECTS_D		:= .objs
TEST_LIB_D			:= utest.h

TEST_SOURCES		:= $(wildcard $(TEST_SOURCES_D)/*.c)
TEST_OBJECTS		:= $(TEST_SOURCES:%.c=$(TEST_OBJECTS_D)/%.o)
TEST_DEPENDENCIES	:= $(TEST_OBJECTS:.o=.d)

CXX					:= gcc
CXXFLAGS			:= -Wall -Wextra -Werror -O3
IFLAGS				:= -I $(TARGET_HEADERS) -I $(TEST_LIB_D)

DIR_DUP			= mkdir -p $(@D)

all: test

-include $(TEST_DEPENDENCIES)

test: $(NAME)
	@(./$(NAME)) && printf " $(MSG_PASSED)" || (printf " $(MSG_FAILED)") 
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

.PHONY: all


RED			=	\033[31m
CYAN		=	\033[36m
BOLD		=	\033[1m
ITALIC		=	\033[3m
RESET		=	\033[0m
MSG_COMPILED	= $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiled	$(BOLD)$@$(RESET) $(CYAN)successfully$(RESET)\n
MSG_FAILED	= \n$(RED)$(BOLD)$(ITALIC)■$(RESET) --- $(BOLD)❌ TESTS $(RED)FAILED ❌$(RESET) ---\n
MSG_PASSED	= \n$(CYAN)$(BOLD)$(ITALIC)■$(RESET) --- $(BOLD)✅ ALL TESTS $(CYAN)PASSED ✅$(RESET) ---\n
