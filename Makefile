# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:53:41 by mel-yous          #+#    #+#              #
#    Updated: 2024/03/16 02:04:13 by aben-nei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address
SRCS = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJS = $(patsubst %.cpp,obj/%.o,$(SRCS))
INC  = $(wildcard *.hpp) $(wildcard */*.hpp)
ALLFLAGS = $(CPPFLAGS) -I config -I utils -I request -I core -I response

TOTAL := $(words $(SRCS))
CURRENT = 0

# ANSI color codes
GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@c++ $(ALLFLAGS) $(OBJS) -o $(NAME)
	@clear
	@echo "\033[1m\033[35mWebserv is ready to use\033[0m"
	@echo "\033[1m\033[32mRun ./webserv webserv.conf\033[0m"

obj/%.o: %.cpp $(INC)
	@mkdir -p $(dir $@)
	@c++ $(ALLFLAGS) -c $< -o $@
	@clear
	@$(eval CURRENT=$(shell echo "$$(($(CURRENT)+1))"))
	@echo "$(CYAN)Compiling$(NC) $< ... $$(($(CURRENT)*100/$(TOTAL)))%$(NC)\r"

clean:
	@rm -rf obj
	@echo "$(YELLOW)Cleaning object files...$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Cleaning executable...$(NC)"

re: clean fclean all

.PHONY: clean server utils config
