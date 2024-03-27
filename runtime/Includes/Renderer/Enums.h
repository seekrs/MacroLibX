/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 22:02:58 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:39:31 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDERER_ENUMS__
#define __MLX_RENDERER_ENUMS__

namespace mlx
{
	enum class BufferType
	{
		Constant = 0,
		HighDynamic, // typically stored in RAM
		LowDynamic,  // typically stored in VRAM

		EndEnum
	};
	constexpr std::size_t BufferTypeCount = static_cast<std::size_t>(BufferType::EndEnum);

	enum class CommandResourceState
	{
		Held = 0,
		Free,

		EndEnum
	};
	constexpr std::size_t CommandResourceStateCount = static_cast<std::size_t>(CommandResourceState::EndEnum);

	enum class CommandBufferState
	{
		Uninit = 0, // buffer not initialized or destroyed
		Ready,      // buffer ready to be used after having been submitted
		Idle,       // buffer has recorded informations but has not been submitted
		Recording,  // buffer is currently recording
		Submitted,  // buffer has been submitted
		
		EndEnum
	};
	constexpr std::size_t CommandBufferStateCount = static_cast<std::size_t>(CommandBufferState::EndEnum);

	enum class CommandBufferType
	{
		SingleTime = 0,
		LongTime,

		EndEnum
	};
	constexpr std::size_t CommandBufferTypeCount = static_cast<std::size_t>(CommandBufferType::EndEnum);
}

#endif
