# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:53:41 by mel-yous          #+#    #+#              #
#    Updated: 2024/02/26 12:08:05 by mel-yous         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = $(wildcard *.cpp */*.cpp)
OBJS = $(SRCS:.cpp=.o)
INC = $(wildcard *.hpp */*.hpp)
COMPILER = c++ -I config/ -I core/ -I Request/ -I utils/

COLOR_GREEN = \033[0;32m
COLOR_RED = \033[0;31m
COLOR_BLUE = \033[0;34m
END_COLOR = \033[0m

RULE = MYRULE

all: $(NAME)

$(NAME): $(OBJS)
	$(COMPILER) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(INC)
	$(COMPILER) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
fclean: clean
	rm -rf $(NAME)
re: clean fclean all