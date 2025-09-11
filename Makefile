STATIC_TARGET 		:= libcerr.a
SHARED_TARGET		:= libcerr.so

DIR_HEADERS		:= headers
DIR_SOURCES		:= sources
DIR_OBJECTS		:= .objs

SOURCES			:= $(DIR_SOURCES)/libcerr.c
OBJECTS			:= $(SOURCES:%.c=$(DIR_OBJECTS)/%.o)
DEPENDENCIES	:= $(OBJECTS:.o=.d)

AR				:= ar
CXX				:= gcc
CXXFLAGS		:= -Wall -Wextra -Werror -fPIC
LDFLAGS			:= -shared
IFLAGS			:= -I $(DIR_HEADERS)

DIR_DUP			= mkdir -p $(@D)


all: $(SHARED_TARGET) $(STATIC_TARGET)

-include $(DEPENDENCIES)

$(DIR_OBJECTS)/%.o: %.c
	@$(DIR_DUP)
	@$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -o $@
	@printf " $(MSG_COMPILING)"

$(SHARED_TARGET): $(OBJECTS)
	@$(CXX) $(LDFLAGS) $(IFLAGS) $^ -o $@
	@printf " $(MSG_COMPILED)"

$(STATIC_TARGET): $(OBJECTS)
	@$(AR) rcs $@ $^
	@printf " $(MSG_COMPILED)"

clean:
	@rm -rf $(DIR_OBJECTS)

fclean: clean
	@rm -rf $(SHARED_TARGET)
	@rm -rf $(STATIC_TARGET)
	@printf " $(MSG_DELETED)$(STATIC_TARGET)$(RESET)\n"
	@printf " $(MSG_DELETED)$(SHARED_TARGET)$(RESET)\n"

re: fclean
	@$(MAKE) -B --no-print-directory

.PHONY: clean fclean re all


RED			=	\033[31m
GREEN		=	\033[32m
BLUE		=	\033[34m
CYAN		=	\033[36m
GRAY		=	\033[90m
BOLD		=	\033[1m
ITALIC		=	\033[3m
RESET		=	\033[0m
MSG_COMPILING	= $(BLUE)$(BOLD)$(ITALIC)■$(RESET) compiling	$(GRAY)$(BOLD)$(ITALIC)$<$(RESET) \n
MSG_COMPILED	= $(CYAN)$(BOLD)$(ITALIC)■$(RESET) compiled	$(BOLD)$@$(RESET) $(CYAN)successfully ✅$(RESET)\n
MSG_DELETED		= $(GRAY)$(BOLD)$(ITALIC)■$(RESET) deleted 	$(GRAY)$(BOLD)$(ITALIC)
