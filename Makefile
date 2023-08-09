# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvaas <vvaas@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2023/08/09 13:44:58 by maldavid         ###   ########.fr        #
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
IMAGES_OPTIMIZED	?= true

CXX			= clang++

ifeq ($(TOOLCHAIN), gcc)
	CXX = g++
endif

CXXFLAGS	= -std=c++17 -O3 -fPIC
INCLUDES	= -I./includes -I./src -I./third_party -I ~/.xmake/packages/l/libsdl/2.26.4/8dfbcb8049e744a597cd5333e1b399cd/include

ifeq ($(DEBUG), true)
	CXXFLAGS += -g -D DEBUG
endif

ifeq ($(IMAGES_OPTIMIZED), true)
	CXXFLAGS += -D IMAGE_OPTIMIZED
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
