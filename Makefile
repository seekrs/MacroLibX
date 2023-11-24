# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2023/11/24 10:03:17 by maldavid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libmlx.so

SRCS		=  $(wildcard $(addsuffix /*.cpp, ./src/core))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/platform))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer))
SRCS		+= $(wildcard $(addsuffix /*.cpp, ./src/renderer/**))

OBJ_DIR		= objs
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

OS = $(shell uname -s)
DEBUG		?= false
TOOLCHAIN	?= clang
IMAGES_OPTIMIZED	?= true
FORCE_INTEGRATED_GPU ?= false
GRAPHICS_MEMORY_DUMP ?= false

MODE	= "release"

CXX		= clang++

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
	MODE = "debug"
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

RM = rm -rf

$(OBJ_DIR)/%.o: %.cpp
	@printf "\033[1;32m[compiling... "$(MODE)" "$(CXX)"]\033[1;00m "$<"\n"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJS)
	@printf "\033[1;32m[linking ... "$(MODE)"]\033[1;00m "$@"\n"
	@$(CXX) -shared -o $(NAME) $(OBJS) $(LDLIBS)
	@printf "\033[1;32m[build finished]\033[1;00m\n"

$(OBJ_DIR):
	@mkdir -p $(sort $(addprefix $(OBJ_DIR)/, $(dir $(SRCS))))
	@printf "\033[1;32m[created objs directory]\033[1;00m\n"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "\033[1;32m[object files removed]\033[1;00m\n"

fclean:		clean
	@$(RM) $(NAME)
	@printf "\033[1;32m["$(NAME)" removed]\033[1;00m\n"

re:			fclean all

.PHONY:		all clean fclean re
