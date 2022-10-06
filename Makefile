# Colors

BOLD_PURPLE	=	\033[1;35m
BOLD_CYAN	=	\033[1;36m
BOLD_YELLOW	=	\033[1;33m
NO_COLOR	=	\033[0m
DEF_COLOR 	= \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

#Sources

SERVER = server 
CLIENT = client

SRCS 		= src/
CLIENT_SRCS = ${SRCS}client.c
SERVER_SRCS = ${SRCS}server.c

CLIENT_OBJS = ${SRCS}client.o
SERVER_OBJS = ${SRCS}server.o

CC = gcc
RM = /bin/rm -f
CFLAGS = -Wall -Wextra -Werror
CFLAGS_D = -Wall -Wextra -Werror -g 

MYLIB_DIR	= 	lib/mylib
MYLIB		=	$(MYLIB_DIR)/mylib.a

# ------------------------------ Messages ------------------------------

COMP_START	=	echo "\n🚧 $(BOLD_YELLOW)Make: $(NO_COLOR)Starting the compilation...\n"
SERV_READY	=	echo "\n📥 Server ready!\n"
CLI_READY	=	echo "\n📟 Client ready!\n"
CLEANED		=	echo "\n💧 $(BOLD_YELLOW)Clean: $(NO_COLOR)Removed all the \".o\" files \n"
FCLEANED	=	echo "\n🧼 $(BOLD_YELLOW)Fclean: $(NO_COLOR)Removed the executables \n"

###

all: libs $(SERVER) $(CLIENT)

libs:
	@$(COMP_START)
	@$(MAKE) -C $(MYLIB_DIR)

$(SERVER): $(SERVER_OBJS) $(MYLIB)
	@echo "\n🚧 $(BOLD_YELLOW)Compiling Server..\n$(NO_COLOR)"
	$(CC) $(CFLAGS) $(SERVER_OBJS) $(MYLIB) -o $(SERVER)
	@$(SERV_READY)

$(CLIENT): $(CLIENT_OBJS) $(MYLIB)
	@echo "\n🚧 $(BOLD_YELLOW)Compiling Client..\n$(NO_COLOR)"
	$(CC) $(CFLAGS) $(CLIENT_OBJS) $(MYLIB) -o $(CLIENT)
	@$(CLI_READY)

$(SRCS)%.o:$(SRCS)%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	@$(MAKE) clean -C $(MYLIB_DIR)
	@$(RM) $(OBJS)

fclean: clean
	@$(MAKE) fclean -C $(MYLIB_DIR)
	@$(RM) $(SERVER) $(CLIENT)

re: fclean all

bonus: 
	@$(MAKE) all

.PHONY: all clean fclean re bonus