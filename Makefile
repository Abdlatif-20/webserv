# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:53:41 by mel-yous          #+#    #+#              #
#    Updated: 2024/02/19 13:22:12 by mel-yous         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS = $(SRCS:.cpp=.o)
INC = $(wildcard *.hpp) $(wildcard */*.hpp)
COMPILER = c++ -g -I config -I utils -I core

COLOR_GREEN = \033[0;32m
COLOR_RED = \033[0;31m
COLOR_BLUE = \033[0;34m
END_COLOR = \033[0m

RULE = MYRULE

all: $(RULE) $(NAME)
	@printf "\n\n"
	@printf "[+] COMPILATION FINISHED\n"
	@printf "[+] EXECUTABLE CREATED \`$(NAME)\`\n"
	@printf "[+] ENJOY !\n"

$(RULE):
	@printf "\n\n$(COLOR_BLUE)██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗\n\
██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║\n\
██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║\n\
██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝\n\
╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ \n\
 ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝$(END_COLOR)\n\n\
[+] COMPILATION IN PROGRESS\n\n"

$(NAME): $(OBJS)
	@$(COMPILER) $(CPPFLAGS) $(OBJS) -o $(NAME)
%.o: %.cpp $(INC)
	@$(COMPILER) $(CPPFLAGS) -c $< -o $@
	@printf "$(COLOR_GREEN)██████$(END_COLOR)"
clean:
	@rm -rf $(OBJS)
fclean: clean
	@rm -rf $(NAME)
re: clean fclean all