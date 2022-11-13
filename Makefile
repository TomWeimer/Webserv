##------------------------------------------------------------------------------##
## 								 THE TARGET										##
##------------------------------------------------------------------------------##
TARGET	:= PROGRAMM

##------------------------------------------------------------------------------##
##								HEADERS											##
##------------------------------------------------------------------------------##
INC = 
INCLUDE	:= $(INC)	

##------------------------------------------------------------------------------##
##								SOURCES											##
##------------------------------------------------------------------------------##
SRC	:=	Server/Server.cpp\
		ManageConnection/ManageConnection.cpp\
		ManageConnection/Register.cpp\
		ManageConnection/SocketSet.cpp\
		Socket/ClientSocket.cpp\
		Socket/ServerSocket.cpp\
		Config/Config.cpp\
		Webserv/Webserv.cpp\
		Parsing/Input/Input.cpp\
		Parsing/Lexer/Lexer.cpp\
		Parsing/Rule/Rule.cpp\
		Parsing/Vocabulary/Vocabulary.cpp\
		Request/Request.cpp\
		Answer/Answer.cpp\
		Answer/AnswerBody.cpp\
		Answer/AnswerHeader.cpp\
		Answer/AnswerStatus.cpp\

SRC_MAIN = main.cpp

SRC_MAIN += $(SRC)

SRC_TEST = tests/main.cpp tests/testSettings.cpp tests/test-utils.cpp tests/Logfile.cpp tests/test_http_parsing.cpp
SRC_TEST += $(SRC)
		

# Src directory
SRC_DIR		:=	./back-end/src/
# Subdirectories of src
SRCS_SUBDIR := ./back-end/src/Server ./back-end/src/Config ./back-end/src/ManageConnection ./back-end/src/Webserv ./back-end/src/Socket ./back-end/src/Request ./back-end/src/Answer   ./back-end/src/Parsing ./back-end/src/Parsing/Vocabulary ./back-end/src/Parsing/Rule ./back-end/src/Parsing/Input ./back-end/src/Parsing/Lexer

# Full paths sources
SRCS			:= $(addprefix $(SRC_DIR), $(SRC_MAIN))
SRCS_TEST		:= $(addprefix $(SRC_DIR), $(SRC_TEST))


##----------------------------------------------------------------------------------##
##								OBJECTS												##
##----------------------------------------------------------------------------------##
OBJ := $(SRCS:.cpp=.o)

OBJ_TEST := $(SRCS_TEST:.cpp=.o)

# Objects directory
OBJ_DIR	:= ./obj/
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
CC		:= c++

# Compilation flags
CFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g
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
    