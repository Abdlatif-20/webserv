# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:53:41 by mel-yous          #+#    #+#              #
#    Updated: 2024/02/21 13:25:11 by houmanso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
INC  = $(wildcard *.hpp) $(wildcard */*.hpp)
OBJS = $(SRCS:.cpp=.o)
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CPPFLAGS += -I config -I utils -I core

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(INC)
	c++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: clean fclean all

.PHONY: clean server utils config
