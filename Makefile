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
MYLIB		=	$(MYLIB_DIR)/libftprintf.a

# ------------------------------ Messages ------------------------------

COMP_START	=	echo "\n๐ง $(BOLD_YELLOW)Make: $(NO_COLOR)Starting the compilation...\n"
SERV_READY	=	echo "๐ฅ Server ready!\n"
CLI_READY	=	echo "๐ Client ready!\n"
CLEANED		=	echo "\n๐ง $(BOLD_YELLOW)Clean: $(NO_COLOR)Removed all the \".o\" files \n"
FCLEANED	=	echo "\n๐งผ $(BOLD_YELLOW)Fclean: $(NO_COLOR)Removed the executables \n"

###

all: 
	@Make libs
	@echo "\n๐ง $(YELLOW)Compiling Server..$(NO_COLOR)"	
	@Make -s $(SERVER)
	@$(SERV_READY)	
	@echo "\n๐ง $(YELLOW)Compiling Client..$(NO_COLOR)"	
	@Make -s $(CLIENT)
	@$(CLI_READY)

libs:
	@echo "\n$(GRAY)โโโโโ    LIBS    โโโโโโ$(NO_COLOR)\n"
	@$(MAKE) -C $(MYLIB_DIR)
	@echo "\n$(GRAY)โโโโโ LIBS  DONE โโโโโโโโโ$(NO_COLOR)\n"	

$(SERVER): $(SERVER_OBJS) $(MYLIB) Makefile
	@echo "๐ $(WHITE)$(CC) $(CFLAGS) $(SERVER_OBJS) $(MYLIB) -o $(CLIENT)$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(SERVER_OBJS) $(MYLIB) -o $(SERVER)


$(CLIENT): $(CLIENT_OBJS) $(MYLIB) Makefile
	@echo "๐ $(WHITE)$(CC) $(CFLAGS) $(CLIENT_OBJS) $(MYLIB) -o $(CLIENT)$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(CLIENT_OBJS) $(MYLIB) -o $(CLIENT)

$(SRCS)%.o:$(SRCS)%.c
	@${CC} ${CFLAGS} -c $< -o $@
	@echo "๐ง $(GRAY)${CC} ${CFLAGS} -I./ -c $< -o $@$(DEF_COLOR)"

clean:
	@$(MAKE) clean -C $(MYLIB_DIR)
	@$(RM) $(SERVER_OBJS) $(CLIENT_OBJS)
	@echo "$(MAGENTA)๐ฝ Cleaned $(SERVER) & $(CLIENT)$(DEF_COLOR)"

fclean: clean
	@$(MAKE) fclean -C $(MYLIB_DIR)
	@$(RM) $(SERVER) $(CLIENT)
	@echo "$(MAGENTA)๐ฝ Fcleaned $(SERVER) & $(CLIENT)$(DEF_COLOR)"	

re: fclean all

bonus: 
	@$(MAKE) all

.PHONY: all clean fclean re bonus