NAME = ircserv
BOT_NAME = IRCBot

SRC_DIR = ./src/
CHANNEL_DIR = ./src/Channel/
COMMANDS_DIR = ./src/Commands/
BOT_DIR = ./src/Bonus/

SRC =	$(SRC_DIR)main.cpp							\
		$(SRC_DIR)Server.cpp						\
		$(SRC_DIR)Client.cpp						\
		$(CHANNEL_DIR)Channel.cpp					\
		$(SRC_DIR)Utils.cpp							\
		$(SRC_DIR)Commands.cpp						\
		$(COMMANDS_DIR)join.cpp						\
		$(COMMANDS_DIR)mode.cpp						\
		$(COMMANDS_DIR)info.cpp						\
		$(COMMANDS_DIR)invite.cpp					\
		$(COMMANDS_DIR)topic.cpp					\
		$(CHANNEL_DIR)ChannelInfo.cpp				\
		$(CHANNEL_DIR)ChannelInvitations.cpp		\
		$(CHANNEL_DIR)ChannelLimits.cpp				\
		$(CHANNEL_DIR)ChannelManagement.cpp			\
		$(CHANNEL_DIR)ChannelOperators.cpp			\

BOT_SRC = $(BOT_DIR)Bot.cpp

PATH_OBJ = ./src/obj/
BOT_PATH_OBJ = ./src/Bonus/obj/
PATH_DEPS = ./src/obj/
BOT_PATH_DEPS = ./src/Bonus/obj/

OBJ = $(patsubst $(SRC_DIR)%.cpp, $(PATH_OBJ)%.o, $(SRC))
BOT_OBJ = $(patsubst $(BOT_DIR)%.cpp, $(BOT_PATH_OBJ)%.o, $(BOT_SRC))

DEPS = $(OBJ:.o=.d)
BONUS_DEPS = $(BOT_OBJ:.o=.d)

INCLUDE = -I./
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g
CC = c++
GREEN = "\033[0;92m"
NC = "\033[37m"

all: $(NAME)

bonus: all $(BOT_NAME)

$(PATH_OBJ)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(BOT_PATH_OBJ)%.o: $(BOT_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo ${GREEN}$(NAME) "compiled!\n"${NC};

$(BOT_NAME): $(BOT_OBJ) 
	$(CC) $(CFLAGS) $(BOT_OBJ) -o $@
	@echo ${GREEN}$(BOT_NAME) "compiled!\n"${NC};

-include $(DEPS)
-include $(BONUS_DEPS)

clean:
	$(RM) $(PATH_OBJ)
	$(RM) $(BOT_PATH_OBJ)
		
fclean: clean
	$(RM) $(NAME) $(BOT_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
#NAME = ircserv
#BOT_NAME = IRCBot

#SRC_DIR = ./src/
#CHANNEL_DIR = ./src/Channel/
#COMMANDS_DIR = ./src/Commands/
#BOT_DIR = ./src/Bonus/

#SRC =	./src/main.cpp							\
#		./src/Server.cpp						\
#		./src/Client.cpp						\
#		./src/Channel.cpp						\
#		./src/Utils.cpp							\
#		./src/Commands.cpp						\
#		$(COMMADS_DIR)join.cpp					\
#		$(COMMADS_DIR)mode.cpp					\
#		$(COMMADS_DIR)info.cpp					\
#		$(COMMADS_DIR)invite.cpp				\
#		$(COMMADS_DIR)topic.cpp					\
#		$(CHANNEL_DIR)Channel.cpp				\
#		$(CHANNEL_DIR)ChanneInfo.cpp			\
#		$(CHANNEL_DIR)ChannelInvitations.cpp	\
#		$(CHANNEL_DIR)ChannelLimits.cpp			\
#		$(CHANNEL_DIR)ChannelManagement.cpp		\
#		$(CHANNEL_DIR)ChannelOperators.cpp		\
		
#BOT_SRC = ./src/Bonus/Bot.cpp

#PATH_OBJ = ./src/obj/
#BOT_PATH_OBJ = ./src/Bonus/obj/
#PATH_DEPS = ./src/obj/
#BOT_PATH_DEPS = ./src/Bonus/obj/

#OBJ = $(addprefix $(PATH_OBJ), $(notdir $(SRC:.cpp=.o)))
#BOT_OBJ = $(addprefix $(BOT_PATH_OBJ), $(notdir $(BOT_SRC:.cpp=.o)))
#OBJ = $(patsubst $(SRC_DIR)%.cpp, $(PATH_OBJ)%.o, $(SRC))
#BOT_OBJ = $(patsubst $(BOT_DIR)%.cpp, $(BOT_PATH_OBJ)%.o, $(BOT_SRC))

#DEPS = $(addprefix $(PATH_DEPS), $(notdir $(SRC:.cpp=.d)))
#BONUS_DEPS = $(addprefix $(BOT_PATH_DEPS), $(notdir $(BOT_SRC:.cpp=.d)))

#INCLUDE = -I./
#RM = rm -rf
#CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g
#CC = c++
#GREEN = "\033[0;92m"
#NC = "\033[37m"

#all: $(NAME)

#bonus: all $(BOT_NAME)

#$(PATH_OBJ)%.o: $(SRC_DIR)%.cpp
#	@mkdir -p $(PATH_OBJ)
#	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

#$(BOT_PATH_OBJ)%.o: $(BOT_DIR)%.cpp
#	@mkdir -p $(BOT_PATH_OBJ)
#	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

#$(NAME): $(OBJ) 
#	$(CC) $(CFLAGS) $(OBJ) -o $@
#	@echo ${GREEN}$(NAME) "compiled!\n"${NC};

#$(BOT_NAME): $(BOT_OBJ) 
#	$(CC) $(CFLAGS) $(BOT_OBJ) -o $@
#	@echo ${GREEN}$(BOT_NAME) "compiled!\n"${NC};

#-include $(DEPS)
#-include $(BONUS_DEPS)

#clean:
#	$(RM) $(PATH_OBJ)
#	$(RM) $(BOT_PATH_OBJ)
		
#fclean: clean
#	$(RM) $(NAME) $(BOT_NAME)

#re: fclean all

#.PHONY: all clean fclean re bonus
