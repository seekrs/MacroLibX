# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2024/03/27 21:30:44 by maldavid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libmlx.so

SRCS =  $(wildcard $(addsuffix /*.cpp, ./runtime/Sources/Core))
SRCS += $(wildcard $(addsuffix /*.cpp, ./runtime/Sources/Platform))
SRCS += $(wildcard $(addsuffix /*.cpp, ./runtime/Sources/Renderer))
SRCS += $(wildcard $(addsuffix /*.cpp, ./runtime/Sources/Renderer/**))
SRCS += $(wildcard $(addsuffix /*.cpp, ./runtime/Sources/Drivers/**))

OBJ_DIR = objs/makefile
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

PCH = ./runtime/Includes/PreCompiled.h
GCH = ./runtime/Includes/PreCompiled.h.gch

OS = $(shell uname -s)
DEBUG ?= false
TOOLCHAIN ?= clang
IMAGES_OPTIMIZED ?= true
FORCE_INTEGRATED_GPU ?= false
GRAPHICS_MEMORY_DUMP ?= false
PROFILER ?= false
LEGACY ?= false

MODE = "release"

CXX = clang++

CXXFLAGS = -std=c++17 -O3 -fPIC -Wall -Wextra -Wno-deprecated -DSDL_MAIN_HANDLED
INCLUDES = -I./includes -I./runtime/Includes -I./third_party

LDLIBS =

ifeq ($(TOOLCHAIN), gcc)
	CXX = g++
endif

ifeq ($(OS), Darwin)
	LDLIBS += -L /opt/homebrew/lib -lSDL2
	CXXFLAGS += -I /opt/homebrew/include
	NAME = libmlx.dylib
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

ifeq ($(PROFILER), true)
	CXXFLAGS += -D PROFILER
endif

ifeq ($(LEGACY), true)
	CXXFLAGS += -D LEGACY
endif

RM = rm -rf

$(OBJ_DIR)/%.o: %.cpp $(GCH)
	@printf "\033[1;32m[compiling... "$(MODE)" "$(CXX)"]\033[1;00m "$<"\n"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(GCH):
	@printf "\033[1;32m[compiling "$(MODE)" "$(CXX)"]\033[1;00m PreCompiled header\n"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(PCH) -o $(GCH)

$(NAME): $(GCH) $(OBJ_DIR) $(OBJS)
	@printf "\033[1;32m[linking ... "$(MODE)"]\033[1;00m "$@"\n"
	@$(CXX) -shared -o $(NAME) $(OBJS) $(LDLIBS)
	@printf "\033[1;32m[build finished]\033[1;00m\n"

$(OBJ_DIR):
	@mkdir -p $(sort $(addprefix $(OBJ_DIR)/, $(dir $(SRCS))))
	@printf "\033[1;32m[created objs directory]\033[1;00m\n"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "\033[1;32m[object files removed]\033[1;00m\n"

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(GCH)
	@printf "\033[1;32m["$(NAME)" and gch removed]\033[1;00m\n"

re: fclean all

.PHONY: all clean fclean re pch
