# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2022/10/04 22:08:52 by maldavid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libMicroX.so

SRCS		=  $(wildcard $(addsuffix /*.cpp, ./src/core))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/platform))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer))

OBJS		= $(SRCS:.cpp=.o)

DEBUG		?= false

CXX			= g++
CXXFLAGS	= -std=c++17 -O3 -fPIC
INCLUDES	= -I./includes -I./src

ifeq ($(DEBUG), true)
	CXXFLAGS += -g
endif

RM			= rm -f

%.o: %.cpp
	@echo "\e[1;32m[compiling...   ]\e[1;00m "$<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	@echo "\e[1;32m[ linking ...]\e[1;00m "$@
	@$(CXX) -shared -o $(NAME) $(OBJS)
	@echo "\e[1;32m[build finished]\e[1;00m"

clean:
	@$(RM) $(OBJS)
	@$(RM) $(OBJS_B)

fclean:		clean
	@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
