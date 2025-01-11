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
SRC =	main.cpp Server.cpp
SRC_DIR = ./src/
PATH_OBJ = ./src/obj/
PATH_DEPS = ./src/obj/
OBJ = $(addprefix $(PATH_OBJ), $(SRC:.cpp=.o))
DEPS = $(addprefix $(PATH_DEPS), $(SRC:.cpp=.d))

INCLUDE = -I./
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g
CC = c++
GREEN = "\033[0;92m"
NC = "\033[37m"

all: $(NAME)

$(PATH_OBJ)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(PATH_OBJ)
	@mkdir -p $(PATH_OBJ)/commands
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

#$(BOT_PATH_OBJ)%.o: $(BOT_SRC_DIR)%.cpp
#	@mkdir -p $(BOT_PATH_OBJ)	
#	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo ${GREEN}$(NAME) "compiled!\n"${NC};
-include $(DEPS)

clean:
	@rm -rf $(PATH_OBJ)
		
fclean:
	@rm -rf $(PATH_OBJ)
	$(RM) $(NAME)

re: fclean all

.PHONY: all bot clean fclean re 
