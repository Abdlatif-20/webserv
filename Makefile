# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:53:41 by mel-yous          #+#    #+#              #
#    Updated: 2024/02/12 00:03:31 by aben-nei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJS = $(patsubst %.cpp,obj/%.o,$(SRCS))
INC = $(wildcard *.hpp) $(wildcard */*.hpp)
COMPILER = c++ -I config -I utils -I Request
MKDIR_P = mkdir -p

all: obj $(NAME)

$(NAME): $(OBJS)
	@$(COMPILER) $(CPPFLAGS) $(OBJS) -o $(NAME)
	@clear
	@echo "\033[1m\033[35mWebserv is ready to use\033[0m"
	@echo "\033[1m\033[32mRun ./webserv webserv.conf\033[0m"

obj/%.o: %.cpp $(INC)
	@$(MKDIR_P) $(dir $@) # dir: get the directory name of the file
	@$(COMPILER) $(CPPFLAGS) -c $< -o $@
	@clear
	@echo "\033[1m\033[32mCompiling $<\033[0m"

obj:
	@$(MKDIR_P) obj
	@echo "\033[1m\033[32mCreating obj directory\033[0m"

clean:
	@rm -rf obj
	@echo "\033[1m\033[31mRemoving obj directory\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1m\033[31mRemoving $(NAME)\033[0m"

re: fclean all
