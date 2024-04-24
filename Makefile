# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/04 16:43:41 by maldavid          #+#    #+#              #
#    Updated: 2024/04/24 14:59:23 by kiroussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME					= libmlx.so
MAKE					= make --no-print-directory

OS 						?= $(shell uname -s)
DEBUG					?= false
TOOLCHAIN				?= clang
IMAGES_OPTIMIZED		?= true
FORCE_INTEGRATED_GPU	?= false
GRAPHICS_MEMORY_DUMP	?= false
PROFILER				?= false
_ENABLEDFLAGS 			=

SRCS					=  $(wildcard $(addsuffix /*.cpp, src/core))
SRCS					+= $(wildcard $(addsuffix /*.cpp, src/platform))
SRCS					+= $(wildcard $(addsuffix /*.cpp, src/renderer))
SRCS					+= $(wildcard $(addsuffix /*.cpp, src/renderer/**))

OBJ_DIR					= objs/make/$(shell echo $(OS) | tr '[:upper:]' '[:lower:]')
OBJS					:= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

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
MODE 					:= $(_RESET)$(_PURPLE)$(_BOLD)Debug$(_RESET)$(_PURPLE)
COLOR					:= $(_PURPLE)
else
MODE 					:= $(_RESET)$(_GREEN)$(_BOLD)Release$(_RESET)$(_GREEN)
COLOR					:= $(_GREEN)
endif

OBJS_TOTAL				= $(words $(OBJS))
N_OBJS					:= $(shell find $(OBJ_DIR) -type f -name '*.o' 2>/dev/null | wc -l)
OBJS_TOTAL				:= $(shell echo $$(( $(OBJS_TOTAL) - $(N_OBJS) )))
CURR_OBJ				= 0

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(eval CURR_OBJ=$(shell echo $$(( $(CURR_OBJ) + 1 ))))
	@$(eval PERCENT=$(shell echo $$(( $(CURR_OBJ) * 100 / $(OBJS_TOTAL) ))))
	@printf "$(COLOR)($(_BOLD)%3s%%$(_RESET)$(COLOR)) $(_RESET)Compiling $(_BOLD)$<$(_RESET)\n" "$(PERCENT)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all:		_printbuildinfos
	@$(MAKE) $(NAME)

$(NAME):	 $(OBJS)
	@printf "Linking $(_BOLD)$(NAME)$(_RESET)\n"
	@$(CXX) -shared -o $(NAME) $(OBJS) $(LDFLAGS)
	@printf "$(_BOLD)$(NAME)$(_RESET) compiled $(COLOR)$(_BOLD)successfully$(_RESET)\n"

_printbuildinfos:
	@printf "$(_PURPLE)$(_BOLD)MacroLibX $(_RESET)Compiling in $(_BOLD)$(MODE)$(_RESET) mode on $(_BOLD)$(OS)$(_RESET) | Using $(_BOLD)$(CXX)$(_RESET), flags: $(_BOLD)$(_ENABLEDFLAGS)$(_RESET)\n"

debug:
	@$(MAKE) all DEBUG=true -j$(shell nproc)

clean:
	@$(RM) $(OBJ_DIR)
	@printf "Cleaned $(_BOLD)$(OBJ_DIR)$(_RESET)\n"

fclean:		clean
	@$(RM) $(NAME)
	@printf "Cleaned $(_BOLD)$(NAME)$(_RESET)\n"

re:			fclean _printbuildinfos
	@$(MAKE) $(NAME)

.PHONY:		all clean debug fclean re
