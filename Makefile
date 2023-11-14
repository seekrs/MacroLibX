# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2023/11/13 06:08:16 by maldavid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libmlx.so

SRCS		=  $(wildcard $(addsuffix /*.cpp, ./src/core))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/platform))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer/**))

OBJS		= $(SRCS:.cpp=.o)

OS = $(shell uname -s)
DEBUG		?= false
TOOLCHAIN	?= clang
IMAGES_OPTIMIZED	?= true
FORCE_INTEGRATED_GPU ?= false
GRAPHICS_MEMORY_DUMP ?= false

CXX			= clang++

ifeq ($(TOOLCHAIN), gcc)
	CXX = g++
endif

CXXFLAGS	= -std=c++17 -O3 -fPIC
INCLUDES	= -I./includes -I./src -I./third_party

LDLIBS =

ifeq ($(OS), Darwin)
	LDLIBS += -lSDL2
endif

ifeq ($(DEBUG), true)
	CXXFLAGS += -g -D DEBUG
endif

ifeq ($(FORCE_INTEGRATED_GPU), true)
	CXXFLAGS += -D FORCE_INTEGRATED_GPU
endif

ifeq ($(IMAGES_OPTIMIZED), true)
	CXXFLAGS += -D IMAGE_OPTIMIZED
endif

ifeq ($(GRAPHICS_MEMORY_DUMP), true)
	CXXFLAGS += -D GRAPHICS_MEMORY_DUMP
endif

RM			= rm -f

%.o: %.cpp
	@echo "\e[1;32m[compiling... "$(CXX)"]\e[1;00m "$<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	@echo "\e[1;32m[linking ...]\e[1;00m "$@
	@$(CXX) -shared -o $(NAME) $(OBJS) $(LDLIBS)
	@echo "\e[1;32m[build finished]\e[1;00m"

clean:
	@$(RM) $(OBJS)

fclean:		clean
	@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
