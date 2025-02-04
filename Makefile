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
SRC_DIR = ./src/
CHANNEL_DIR = ./src/Channel/
SRC = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(CHANNEL_DIR)*.cpp)

PATH_OBJ = ./src/obj/
PATH_DEPS = ./src/obj/
OBJ = $(addprefix $(PATH_OBJ), $(notdir $(SRC:.cpp=.o)))
DEPS = $(addprefix $(PATH_DEPS), $(notdir $(SRC:.cpp=.d)))

INCLUDE = -I./
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g
CC = c++
GREEN = "\033[0;92m"
NC = "\033[37m"

all: $(NAME)

$(PATH_OBJ)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(PATH_OBJ)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(PATH_OBJ)%.o: $(CHANNEL_DIR)%.cpp
	@mkdir -p $(PATH_OBJ)
	$(CC) $(CFLAGS) -MMD $(INCLUDE) -c $< -o $@

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo ${GREEN}$(NAME) "compiled!\n"${NC};
-include $(DEPS)

clean:
	@rm -rf $(PATH_OBJ)
		
fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
