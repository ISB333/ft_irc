#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adesille <adesille@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/21 11:35:08 by adesille          #+#    #+#              #
#    Updated: 2025/04/23 14:09:27 by adesille         ###   ########.fr        #
#                                                                              #
#******************************************************************************#


NAME		= ircserv
CXXFLAGS		+= -Wall -Wextra -Werror -I./includes -I./includes/Classes -std=c++98
CXX			= c++

HIDE_CURSOR		= \033[?25l
SHOW_CURSOR		= \033[?25h
CYAN			= \033[36m
RESET			= \033[0m
CARRIAGE_RETURN	= \r

DEFAULT = \033[0;39m
BLUE = \033[0;34m
DARK_GREEN =\033[0;3;32m
RED = \033[31;1m
WHITE = \033[0;37m
YELLOW = \033[0;33m

SRC = ./srcs
OBJ_DIR = .obj

SRCS := $(SRC)/main.cpp $(SRC)/Server.cpp $(SRC)/Client.cpp $(SRC)/Channel.cpp 
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	@if [ ! -z "$(wildcard .obj/*.o)" ]; then \
		echo "$(YELLOW)\nCompiling ircserv files...$(DEFAULT)"; \
	fi
	@$(CXX) $(CXXFLAGS) -c $< -o $@


clean :
# @rm -rf $(OBJ_DIR) $(DEPFILES)
	rm -rf $(OBJS)
	@echo "$(DARK_GREEN)\nEvery files are cleaned$(DEFAULT)"

fclean : clean
	@rm -f $(NAME)

	
-include $(DEPFILES)

disco:
	@bash ./.scripts/.disco.sh

git:
	@git add .
	@git commit -m "$(shell date +"%Y-%m-%d %H:%M:%S")'s commit"
	@git push

x:	
	@make
	./ircserv

.PHONY: all bonus clean disco git x fclean re
