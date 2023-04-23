# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2023/04/22 20:28:48 by maldavid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libmlx.so

SRCS		=  $(wildcard $(addsuffix /*.cpp, ./src/core))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/platform))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer/**))

OBJS		= $(SRCS:.cpp=.o)

DEBUG		?= false
TOOLCHAIN	?= clang

CXX			= clang++

ifeq ($(TOOLCHAIN), gcc)
	CXX = g++
endif

CXXFLAGS	= -std=c++17 -O3 -fPIC
INCLUDES	= -I./includes -I./src -I./third_party

ifeq ($(DEBUG), true)
	CXXFLAGS += -g -D DEBUG
endif

RM			= rm -f

%.o: %.cpp
	@echo "\e[1;32m[compiling... "$(CXX)"]\e[1;00m "$<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	@echo "\e[1;32m[linking ...]\e[1;00m "$@
	@$(CXX) -shared -o $(NAME) $(OBJS)
	@echo "\e[1;32m[build finished]\e[1;00m"

clean:
	@$(RM) $(OBJS)

fclean:		clean
	@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
