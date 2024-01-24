/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_profile.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 08:49:17 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/03 15:33:35 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_PROFILE__
#define __MLX_PROFILE__

// Try to identify the compiler
#if defined(__BORLANDC__)
	#define MLX_COMPILER_BORDLAND
#elif defined(__clang__)
	#define MLX_COMPILER_CLANG
	#ifdef __MINGW32__
		#define MLX_COMPILER_MINGW
		#ifdef __MINGW64_VERSION_MAJOR
			#define MLX_COMPILER_MINGW_W64
		#endif
	#endif
#elif defined(__GNUC__) || defined(__MINGW32__)
	#define MLX_COMPILER_GCC
	#ifdef __MINGW32__
		#define MLX_COMPILER_MINGW
		#ifdef __MINGW64_VERSION_MAJOR
			#define MLX_COMPILER_MINGW_W64
		#endif
	#endif
#elif defined(__INTEL_COMPILER) || defined(__ICL)
	#define MLX_COMPILER_INTEL
#elif defined(_MSC_VER)
	#define MLX_COMPILER_MSVC
#else
	#define MLX_COMPILER_UNKNOWN
	#warning "This compiler is not fully supported"
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
	#define MLX_PLAT_WINDOWS
#elif defined(__linux__)
	#define MLX_PLAT_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
	#define MLX_PLAT_MACOS
#elif defined(unix) || defined(__unix__) || defined(__unix)
	#define MLX_PLAT_UNIX
#else
	#error "Unknown environment (not Windows, not Linux, not MacOS, not Unix)"
#endif

#ifdef MLX_PLAT_WINDOWS
	#ifdef MLX_COMPILER_MSVC
		#ifdef MLX_BUILD
			#define MLX_API __declspec(dllexport)
		#else
			#define MLX_API __declspec(dllimport)
		#endif
	#elif defined(MLX_COMPILER_GCC)
		#ifdef MLX_BUILD
			#define MLX_API __attribute__((dllexport))
		#else
			#define MLX_API __attribute__((dllimport))
		#endif
	#else
		#define MLX_API
	#endif
#elif defined(MLX_COMPILER_GCC)
	#define MLX_API __attribute__((visibility("default")))
#else
	#define MLX_API
#endif

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define MLX_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define MLX_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
	#define MLX_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define MLX_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define MLX_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define MLX_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define MLX_FUNC_SIG __func__
#else
	#define MLX_FUNC_SIG "Unknown function"
#endif

#ifndef __cplusplus // if we compile in C
	#ifdef __STDC__
		#ifdef __STDC_VERSION__
			#if __STDC_VERSION__ == 199409L
				#define MLX_C_VERSION 1994
			#elif __STDC_VERSION__ == 199901L
				#define MLX_C_VERSION 1999
			#elif __STDC_VERSION__ == 201112L
				#define MLX_C_VERSION 2011
			#elif __STDC_VERSION__ == 201710L
				#define MLX_C_VERSION 2017
			#elif __STDC_VERSION__ == 202311L
				#define MLX_C_VERSION 2023
			#else
				#define MLX_C_VERSION 0
			#endif
		#else
			#define MLX_C_VERSION 0
		#endif
	#else
		#define MLX_C_VERSION 0
	#endif
#else
	#define MLX_C_VERSION 0
#endif

#if defined(MLX_PLAT_WINDOWS)
	#define VK_USE_PLATFORM_WIN32_KHR
	#ifdef __cplusplus
		constexpr const char* VULKAN_LIB_NAME = "vulkan-1.dll";
	#endif
#elif defined(MLX_PLAT_MACOS)
	#define VK_USE_PLATFORM_MACOS_MVK
	#define VK_USE_PLATFORM_METAL_EXT
	#ifdef __cplusplus
		constexpr const char* VULKAN_LIB_NAME = "libvulkan.dylib / libvulkan.1.dylib / libMoltenVK.dylib";
	#endif
#else
	#define VK_USE_PLATFORM_XLIB_KHR
	#define VK_USE_PLATFORM_WAYLAND_KHR
	#ifdef __cplusplus
		constexpr const char* VULKAN_LIB_NAME = "libvulkan.so / libvulkan.so.1";
	#endif
#endif

// Checking common assumptions
#ifdef __cplusplus
	#include <climits>
	#include <cstdint>

	static_assert(CHAR_BIT == 8, "CHAR_BIT is expected to be 8");

	static_assert(sizeof(int8_t)  == 1, "int8_t is not of the correct size" );
	static_assert(sizeof(int16_t) == 2, "int16_t is not of the correct size");
	static_assert(sizeof(int32_t) == 4, "int32_t is not of the correct size");
	static_assert(sizeof(int64_t) == 8, "int64_t is not of the correct size");

	static_assert(sizeof(uint8_t)  == 1, "uint8_t is not of the correct size" );
	static_assert(sizeof(uint16_t) == 2, "uint16_t is not of the correct size");
	static_assert(sizeof(uint32_t) == 4, "uint32_t is not of the correct size");
	static_assert(sizeof(uint64_t) == 8, "uint64_t is not of the correct size");
#elif MLX_C_VERSION >= 2011
	#if MLX_C_VERSION < 2023
		#include <assert.h>
	#endif
	#include <limits.h>
	#include <stdint.h>

	static_assert(CHAR_BIT == 8, "CHAR_BIT is expected to be 8");

	static_assert(sizeof(int8_t)  == 1, "int8_t is not of the correct size" );
	static_assert(sizeof(int16_t) == 2, "int16_t is not of the correct size");
	static_assert(sizeof(int32_t) == 4, "int32_t is not of the correct size");
	static_assert(sizeof(int64_t) == 8, "int64_t is not of the correct size");

	static_assert(sizeof(uint8_t)  == 1, "uint8_t is not of the correct size" );
	static_assert(sizeof(uint16_t) == 2, "uint16_t is not of the correct size");
	static_assert(sizeof(uint32_t) == 4, "uint32_t is not of the correct size");
	static_assert(sizeof(uint64_t) == 8, "uint64_t is not of the correct size");
#else
	#define STATIC_ASSERT(COND, MSG) typedef char static_assertion___##MSG[(COND)?1:-1]

	#include <limits.h>
	#include <stdint.h>

	STATIC_ASSERT(CHAR_BIT == 8, CHAR_BIT_is_expected_to_be_8);

	STATIC_ASSERT(sizeof(int8_t)  == 1,  int8_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(int16_t) == 2, int16_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(int32_t) == 4, int32_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(int64_t) == 8, int64_t_is_not_of_the_correct_size);

	STATIC_ASSERT(sizeof(uint8_t)  == 1,  uint8_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(uint16_t) == 2, uint16_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(uint32_t) == 4, uint32_t_is_not_of_the_correct_size);
	STATIC_ASSERT(sizeof(uint64_t) == 8, uint64_t_is_not_of_the_correct_size);
#endif

#endif
