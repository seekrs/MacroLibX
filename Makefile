# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2024/04/23 20:38:01 by kiroussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME					= libmlx.so
MAKE					= make --no-print-directory

SRCS					=  $(wildcard $(addsuffix /*.cpp, src/core))
SRCS					+= $(wildcard $(addsuffix /*.cpp, src/platform))
SRCS					+= $(wildcard $(addsuffix /*.cpp, src/renderer))
SRCS					+= $(wildcard $(addsuffix /*.cpp, src/renderer/**))

OBJ_DIR					= objs
OBJS					:= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

OS 						:= $(shell uname -s)
DEBUG					?= false
TOOLCHAIN				?= clang
IMAGES_OPTIMIZED		?= true
FORCE_INTEGRATED_GPU	?= false
GRAPHICS_MEMORY_DUMP	?= false
PROFILER				?= false
_ENABLEDFLAGS 			=

CXX						= clang++
CXXFLAGS				= -std=c++17 -O3 -fPIC -Wall -Wextra -Werror -DSDL_MAIN_HANDLED
INCLUDES				= -I./includes -I./src -I./third_party

ifeq ($(TOOLCHAIN), gcc)
CXX						= g++
CXXFLAGS				+= -Wno-error=cpp
else
CXXFLAGS				+= -Wno-error=#warning
endif

ifeq ($(OS), Darwin)
LDFLAGS					+= -L /opt/homebrew/lib -lSDL2
CXXFLAGS				+= -I /opt/homebrew/include
NAME					= libmlx.dylib
endif

ifeq ($(DEBUG), true)
CXXFLAGS				+= -g3 -D DEBUG
LDFLAGS					+= -rdynamic
endif

ifeq ($(FORCE_INTEGRATED_GPU), true)
_ENABLEDFLAGS 			+= FORCE_INTEGRATED_GPU
endif

ifeq ($(IMAGES_OPTIMIZED), true)
_ENABLEDFLAGS 			+= IMAGE_OPTIMIZED
endif

ifeq ($(GRAPHICS_MEMORY_DUMP), true)
_ENABLEDFLAGS 			+= GRAPHICS_MEMORY_DUMP
endif

ifeq ($(PROFILER), true)
_ENABLEDFLAGS 			+= PROFILER
endif

CXXFLAGS 				+= $(addprefix -D, $(_ENABLEDFLAGS))

RM = rm -rf

TPUT 					= tput -T xterm-256color
_RESET 					:= $(shell $(TPUT) sgr0)
_BOLD 					:= $(shell $(TPUT) bold)
_ITALIC 				:= $(shell $(TPUT) sitm)
_UNDER 					:= $(shell $(TPUT) smul)
_GREEN 					:= $(shell $(TPUT) setaf 2)
_YELLOW 				:= $(shell $(TPUT) setaf 3)
_RED 					:= $(shell $(TPUT) setaf 1)
_GRAY 					:= $(shell $(TPUT) setaf 8)
_PURPLE 				:= $(shell $(TPUT) setaf 5)

ifeq ($(DEBUG), true)
MODE 					:= $(_RESET)$(_PURPLE)$(_BOLD)Debug$(_RESET)
else
MODE 					:= $(_RESET)$(_GREEN)$(_BOLD)Release$(_RESET)
endif

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "$(MODE) $(_GRAY)|$(_RESET) Compiling $(_BOLD)$<$(_RESET) $(_GRAY)$(_ITALIC)($@)$(_RESET)\n"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	@printf "$(MODE) $(_GRAY)|$(_RESET) Linking $(_BOLD)$(NAME)$(_RESET) $(_GRAY)$(_ITALIC)(from ./$(OBJ_DIR)/)$(_RESET)\n"
	@$(CXX) -shared -o $(NAME) $(OBJS) $(LDFLAGS)
	@printf "$(_BOLD)$(NAME)$(_RESET) compiled $(_GREEN)$(_BOLD)successfully$(_RESET)\n"

_printbuildinfos:
	@printf "Building $(_BOLD)$(NAME)$(_RESET) in $(_BOLD)$(MODE)$(_RESET) mode\n"
	@printf "\tOperating System: $(_BOLD)$(OS)$(_RESET)\n"
	@printf "\tCompiler: $(_BOLD)$(CXX)$(_RESET)\n"
	@printf "\tEnabled flags: $(_BOLD)$(_ENABLEDFLAGS)$(_RESET)\n"
	@printf "\tCXXFLAGS: $(_BOLD)$(CXXFLAGS)$(_RESET)\n"
	@printf "\tINCLUDES: $(_BOLD)$(INCLUDES)$(_RESET)\n"
	@printf "\tLDFLAGS: $(_BOLD)$(LDFLAGS)$(_RESET)\n"

debug:
	@$(MAKE) _printbuildinfos DEBUG=true
	@$(MAKE) all DEBUG=true -j

clean:
	@$(RM) $(OBJ_DIR)
	@printf "Cleaned $(_BOLD)$(OBJ_DIR)$(_RESET)\n"

fclean:		clean
	@$(RM) $(NAME)
	@printf "Cleaned $(_BOLD)$(NAME)$(_RESET)\n"

re:			fclean all

.PHONY:		all clean debug fclean re
