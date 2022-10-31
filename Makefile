##------------------------------------------------------------------------------##
## 								 THE TARGET										##
##------------------------------------------------------------------------------##
TARGET	:= server

##------------------------------------------------------------------------------##
##								HEADERS											##
##------------------------------------------------------------------------------##
INC = -I ./back-end/core/include/ -I ./back-end/core/tests/include -I ./back-end/core/src/MyParsing/include
INCLUDE	:= $(INC)	

##------------------------------------------------------------------------------##
##								SOURCES											##
##------------------------------------------------------------------------------##
SRC	:=	src/http/Answer.cpp\
		src/http/Request.cpp\
		src/server/Server.cpp\
		src/server/Socket.cpp\
		src/server/config/Settings.cpp\
		src/utils/utils.cpp\

SRC_PARSER := src/MyParsing/src/Lexer/Vocabulary.cpp src/MyParsing/src/Lexer/Lexer.cpp src/MyParsing/src/Lexer/Input.cpp  src/MyParsing/src/Lexer/Rule.cpp

SRC_MAIN = src/main.cpp

SRC_MAIN += $(SRC)
SRC_MAIN += $(SRC_PARSER)

SRC_TEST = tests/src/main.cpp tests/src/testSettings.cpp tests/src/test-utils.cpp tests/src/Logfile.cpp tests/src/test_http_parsing.cpp
SRC_TEST += $(SRC)
SRC_TEST += $(SRC_PARSER)
		

# Src directory
SRC_DIR		:=	./back-end/core/
# Subdirectories of src
SRCS_SUBDIR := ./back-end/core/src ./back-end/core/src/http ./back-end/core/src/server ./back-end/core/src/server/config ./back-end/core/src/utils ./back-end/core/src/http ./back-end/core/tests/src ./back-end/core/src/MyParsing/src/Lexer 

# Full paths sources
SRCS			:= $(addprefix $(SRC_DIR), $(SRC_MAIN))
SRCS_TEST		:= $(addprefix $(SRC_DIR), $(SRC_TEST))


##----------------------------------------------------------------------------------##
##								OBJECTS												##
##----------------------------------------------------------------------------------##
OBJ := $(SRCS:.cpp=.o)

OBJ_TEST := $(SRCS_TEST:.cpp=.o)

# Objects directory
OBJ_DIR	:= obj/
# From str to obj/
OBJS := $(subst $(SRC_DIR), $(OBJ_DIR), $(OBJ))
OBJS_TEST := $(subst $(SRC_DIR), $(OBJ_DIR), $(OBJ_TEST))


##----------------------------------------------------------------------------------##
##							 OBJ_DIR CREATION										##
##----------------------------------------------------------------------------------##
MKDIR_P	:= mkdir -p
# Objects directory creation
NEWSUBDIR = $(MKDIR_P) $(addsuffix ;, $(subst $(SRC_DIR), $(OBJ_DIR), $(listObj)))


##------------------------------------------------------------------------------##
##									PRINT										##
##------------------------------------------------------------------------------##
ECHO	:= echo
# colors
RED		:= \033[1;31m
YEL		:= \033[1;33m
BLU		:= \033[1;34m
WHITE	:= \033[1;37m
GREEN	:= \033[1;32m
RESET	:= \033[0m


##------------------------------------------------------------------------------##
##								COMPILATION										##
##------------------------------------------------------------------------------##
CC		:= g++-12

# Compilation flags
CFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g -fsanitize=leak
# explicit compilation
$(OBJ_DIR)%.o: %.cpp 
	@$(CC) $(CFLAGS) $(INCLUDE)  -c $< -o $@
# vpath
VPATH := $(SRC_DIR) $(OBJ_DIR) $(SRCS_SUBDIR)


##------------------------------------------------------------------------------##
##									MAKE RULES									##
##------------------------------------------------------------------------------##
all:	$(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	@ mkdir -p $(OBJ_DIR)
	@$(foreach listObj, $(SRCS_SUBDIR), $(NEWSUBDIR))

$(TARGET):	$(SRCS) $(OBJS)
	@ $(ECHO) "$(GREEN) $(TARGET) compiled with success!$(RESET)"
	@ $(CC) $(CFLAGS) $(OBJS) -o $(TARGET) 
tests: $(OBJ_DIR) $(SRCS_TEST) $(OBJS_TEST)
	@ $(ECHO) "$(GREEN) $(TARGET) compiled with success!$(RESET)"
	@ $(CC) $(CFLAGS) $(OBJS_TEST) -o $(TARGET)

clean:
	@ $(ECHO) "\n $(RED)removing:\n		$(RESET) $(TARGET) *.o $(RESET)"
	@ rm -rf $(OBJS)
	@ rm -rf $(OBJ_DIR)

fclean: clean
	@ $(ECHO) "		$(RESET) $(TARGET)\n"
	@ rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re tests
    