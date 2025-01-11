NAME = libmlx.so
MAKE = make --no-print-directory

OS ?= $(shell uname -s)
DEBUG ?= false
TOOLCHAIN ?= clang
IMAGES_OPTIMIZED ?= true
FORCE_INTEGRATED_GPU ?= false
GRAPHICS_MEMORY_DUMP ?= false
PROFILER ?= false
FORCE_WAYLAND ?= false
DISABLE_ALL_SAFETIES ?= false
_ENABLEDFLAGS =

SRCS =  $(wildcard $(addsuffix /*.cpp, runtime/Sources/Core))
SRCS += $(wildcard $(addsuffix /*.cpp, runtime/Sources/Graphics))
SRCS += $(wildcard $(addsuffix /*.cpp, runtime/Sources/Platform))
SRCS += $(wildcard $(addsuffix /*.cpp, runtime/Sources/Renderer))
SRCS += $(wildcard $(addsuffix /*.cpp, runtime/Sources/Renderer/Vulkan))
SRCS += $(wildcard $(addsuffix /*.cpp, runtime/Sources/Renderer/Pipelines))
SRCS += $(wildcard $(addsuffix /*.cpp, runtime/Sources/Renderer/RenderPasses))

OBJ_DIR = objs/make/$(shell echo $(OS) | tr '[:upper:]' '[:lower:]')
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

SHADERS_DIR = runtime/Includes/Embedded
SHADERS_SRCS = $(wildcard $(addsuffix /*.nzsl, $(SHADERS_DIR)))
SPVS = $(SHADERS_SRCS:.nzsl=.spv.h)

CXX = clang++
CXXFLAGS = -std=c++20 -fPIC -Wall -Wextra -DSDL_MAIN_HANDLED
INCLUDES = -I./includes -I./runtime/Includes -I./runtime/Sources -I./third_party

CXXPCHFLAGS = -xc++-header -std=c++20 -Wall -Wextra 

PCH_SOURCE = runtime/Includes/PreCompiled.h
GCH = runtime/Includes/PreCompiled.h.gch
CCH = runtime/Includes/PreCompiled.h.pch
PCH =

NZSLC ?= nzslc

ifeq ($(TOOLCHAIN), gcc)
	CXX = g++
	PCH = $(GCH)
	CXXFLAGS += -Wno-error=cpp
else
	PCH = $(CCH)
	CXXFLAGS += -Wno-error=#warning -include-pch $(PCH)
endif

ifeq ($(OS), Darwin)
	LDFLAGS += -L /opt/homebrew/Cellar/lib -L /usr/local/Cellar -L /opt/homebrew/lib -lSDL2
	INCLUDES += -I /opt/homebrew/Cellar/include -I /usr/local/Cellar/include -I /opt/homebrew/include
	NAME = libmlx.dylib
endif

ifeq ($(DEBUG), true)
	CXXFLAGS += -g3 -O0 -D DEBUG
	LDFLAGS += -rdynamic
else
	CXXFLAGS += -O3
endif

ifeq ($(FORCE_INTEGRATED_GPU), true)
	_ENABLEDFLAGS += FORCE_INTEGRATED_GPU
endif

ifeq ($(IMAGES_OPTIMIZED), true)
	_ENABLEDFLAGS += IMAGE_OPTIMIZED
endif

ifeq ($(GRAPHICS_MEMORY_DUMP), true)
	_ENABLEDFLAGS += GRAPHICS_MEMORY_DUMP
endif

ifeq ($(PROFILER), true)
	_ENABLEDFLAGS += PROFILER
endif

ifeq ($(FORCE_WAYLAND), true)
	_ENABLEDFLAGS += FORCE_WAYLAND
endif

ifeq ($(DISABLE_ALL_SAFETIES), true)
	_ENABLEDFLAGS += DISABLE_ALL_SAFETIES
endif

CXXFLAGS += $(addprefix -D, $(_ENABLEDFLAGS))

RM = rm -rf

TPUT = tput -T xterm-256color
_RESET := $(shell $(TPUT) sgr0)
_BOLD := $(shell $(TPUT) bold)
_ITALIC := $(shell $(TPUT) sitm)
_UNDER := $(shell $(TPUT) smul)
_GREEN := $(shell $(TPUT) setaf 2)
_YELLOW := $(shell $(TPUT) setaf 3)
_RED := $(shell $(TPUT) setaf 1)
_GRAY := $(shell $(TPUT) setaf 8)
_PURPLE := $(shell $(TPUT) setaf 5)

ifeq ($(DEBUG), true)
	MODE := $(_RESET)$(_PURPLE)$(_BOLD)Debug$(_RESET)$(_PURPLE)
	COLOR := $(_PURPLE)
else
	MODE := $(_RESET)$(_GREEN)$(_BOLD)Release$(_RESET)$(_GREEN)
	COLOR := $(_GREEN)
endif

OBJS_TOTAL = $(words $(OBJS))
N_OBJS := $(shell find $(OBJ_DIR) -type f -name '*.o' 2>/dev/null | wc -l)
OBJS_TOTAL := $(shell echo $$(( $(OBJS_TOTAL) - $(N_OBJS) )))
ifeq ($(OBJS_TOTAL), 0) # To avoid division per 0
	OBJS_TOTAL := 1
endif
CURR_OBJ = 0

$(OBJ_DIR)/%.o: %.cpp $(PCH)
	@mkdir -p $(dir $@)
	@$(eval CURR_OBJ=$(shell echo $$(( $(CURR_OBJ) + 1 ))))
	@$(eval PERCENT=$(shell echo $$(( $(CURR_OBJ) * 100 / $(OBJS_TOTAL) ))))
	@printf "$(COLOR)($(_BOLD)%3s%%$(_RESET)$(COLOR)) $(_RESET)Compiling $(_BOLD)$<$(_RESET)\n" "$(PERCENT)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

SPVS_TOTAL = $(words $(SPVS))
N_SPVS := $(shell find $(SHADERS_DIR) -type f -name '*.spv.h' 2>/dev/null | wc -l)
SPVS_TOTAL := $(shell echo $$(( $(SPVS_TOTAL) - $(N_SPVS) )))
ifeq ($(SPVS_TOTAL), 0) # Same
	SPVS_TOTAL := 1
endif
CURR_SPV = 0

%.spv.h: %.nzsl
	@$(eval CURR_SPV=$(shell echo $$(( $(CURR_SPV) + 1 ))))
	@$(eval PERCENT=$(shell echo $$(( $(CURR_SPV) * 100 / $(SPVS_TOTAL) ))))
	@printf "$(COLOR)($(_BOLD)%3s%%$(_RESET)$(COLOR)) $(_RESET)Compiling $(_BOLD)$<$(_RESET)\n" "$(PERCENT)"
	@$(NZSLC) --compile=spv-header $< -o $(SHADERS_DIR) --optimize

all: _printbuildinfos
	@$(MAKE) $(NAME)

$(PCH):
	@printf "$(COLOR)($(_BOLD)%3s%%$(_RESET)$(COLOR)) $(_RESET)Compiling $(_BOLD)PreCompiled header$(_RESET)\n" "0"
	@$(CXX) $(CXXPCHFLAGS) $(INCLUDES) $(PCH_SOURCE) -o $(PCH)

$(NAME): $(OBJS)
	@printf "Linking $(_BOLD)$(NAME)$(_RESET)\n"
	@$(CXX) -shared -o $(NAME) $(OBJS) $(LDFLAGS)
	@printf "$(_BOLD)$(NAME)$(_RESET) compiled $(COLOR)$(_BOLD)successfully$(_RESET)\n"

_printbuildinfos:
	@printf "$(_PURPLE)$(_BOLD)MacroLibX $(_RESET)Compiling in $(_BOLD)$(MODE)$(_RESET) mode on $(_BOLD)$(OS)$(_RESET) | Using $(_BOLD)$(CXX) ($(shell $(CXX) --version | head -n 1))$(_RESET), flags: $(_BOLD)$(_ENABLEDFLAGS)$(_RESET)\n"

debug:
	@$(MAKE) all DEBUG=true -j$(shell nproc)

clean-shaders:
	@$(RM) $(SPVS)

shaders: clean-shaders $(SPVS)

clean:
	@$(RM) $(OBJ_DIR)
	@printf "Cleaned $(_BOLD)$(OBJ_DIR)$(_RESET)\n"
	@$(RM) $(GCH)
	@$(RM) $(CCH)
	@printf "Cleaned pre compiled header\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "Cleaned $(_BOLD)$(NAME)$(_RESET)\n"

re: fclean _printbuildinfos
	@$(MAKE) $(NAME)

.PHONY: all clean debug shaders clean-shaders fclean re
