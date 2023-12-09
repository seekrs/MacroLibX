/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profile.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 08:49:17 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/08 18:49:38 by kbz_8            ###   ########.fr       */
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
	#error "Unknown environment!"
#endif


// Checking common assumptions
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

#endif
