#ifndef __MLX_PRE_COMPILED_HEADER__
#define __MLX_PRE_COMPILED_HEADER__

#define VK_NO_PROTOTYPES

#define Window X11Window // f*ck X11

#include <mlx_profile.h>
#include <mlx.h>
#include <cstdio>
#include <cstdarg>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <Renderer/Vulkan/VulkanPrototypes.h>
#include <kvf.h>

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
#include <random>
#include <concepts>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <stb_truetype.h>
#include <variant>
#if defined(MLX_PLAT_LINUX)
	#include <math.h> // sincos
#endif

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

#undef Window

#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <Core/Profiler.h>
#include <Utils/NonOwningPtr.h>

using Handle = void*;

#endif
