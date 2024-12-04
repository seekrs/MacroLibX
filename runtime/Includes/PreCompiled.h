#ifndef __MLX_PRE_COMPILED_HEADER__
#define __MLX_PRE_COMPILED_HEADER__

#define VK_NO_PROTOTYPES

#include <mlx_profile.h>
#include <mlx.h>
#include <cstdio>
#include <cstdarg>
#include <iostream>

#include <vulkan/vulkan.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <functional>
#include <memory>
#include <list>
#include <map>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <string>
#include <string_view>
#include <chrono>
#include <unordered_set>
#include <function.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <set>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <random>
#include <concepts>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <stb_truetype.h>
#include <variant>
#include <type_traits>
#include <string_view>
#include <sstream>
#include <ostream>
#include <ranges>

#ifndef MLX_PLAT_WINDOWS
	#include <dlfcn.h>
#endif

#ifdef MLX_PLAT_LINUX
	#include <math.h> // sincos
#endif

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1000000
#define VMA_ASSERT(expr) ((void)0) // Because why not

#ifdef MLX_COMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weverything"
		#include <vma.h>
	#pragma clang diagnostic pop
#elif defined(MLX_COMPILER_GCC)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wparentheses"
		#include <vma.h>
	#pragma GCC diagnostic pop
#else
	#include <vma.h>
#endif

#include <Utils/AntiX11.h>
#include <Utils/AntiWindows.h>

#define KVF_IMPL_VK_NO_PROTOTYPES
#ifdef DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif
#include <kvf.h>

#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <Core/Profiler.h>
#include <Utils/NonOwningPtr.h>
#include <Utils/NonCopyable.h>

constexpr const int RANGE = 1024;

using Handle = void*;

#endif
