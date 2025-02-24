# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcatalan@student.42barcelona.com <mcata    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/16 08:03:29 by mcatalan@st       #+#    #+#              #
#    Updated: 2025/01/11 18:23:40 by jpaul-kr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
BOT_NAME = IRCBot

SRC_DIR = ./src/
CHANNEL_DIR = ./src/Channel/
BOT_DIR = ./src/Bonus/

SRC = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(CHANNEL_DIR)*.cpp)
BOT_SRC = $(wildcard $(BOT_DIR)*.cpp)

PATH_OBJ = ./src/obj/
BOT_PATH_OBJ = ./src/Bonus/obj/
PATH_DEPS = ./src/obj/
BOT_PATH_DEPS = ./src/Bonus/obj/

OBJ = $(addprefix $(PATH_OBJ), $(notdir $(SRC:.cpp=.o)))
BOT_OBJ = $(addprefix $(BOT_PATH_OBJ), $(notdir $(BOT_SRC:.cpp=.o)))

DEPS = $(addprefix $(PATH_DEPS), $(notdir $(SRC:.cpp=.d)))
BONUS_DEPS = $(addprefix $(BOT_PATH_DEPS), $(notdir $(BOT_SRC:.cpp=.d)))

INCLUDE = -I./
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g
CC = c++
GREEN = "\033[0;92m"
NC = "\033[37m"

all: $(NAME)

bonus: all $(BOT_NAME)

$(PATH_OBJ)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(PATH_OBJ)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(PATH_OBJ)%.o: $(CHANNEL_DIR)%.cpp
	@mkdir -p $(PATH_OBJ)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(BOT_PATH_OBJ)%.o: $(BOT_DIR)%.cpp
	@mkdir -p $(BOT_PATH_OBJ)
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
