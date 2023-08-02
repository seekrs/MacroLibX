/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:16:32 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/23 12:31:42 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

#include <volk.h>
#include <optional>

#include "vk_queues.h"
#include "vk_device.h"
#include "vk_instance.h"
#include "vk_validation_layers.h"

#include <utils/singleton.h>
#include <core/errors.h>

namespace mlx
{
	namespace RCore
	{
		std::optional<uint32_t> findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, bool error = true);
	}

	#ifdef DEBUG
        constexpr const bool enableValidationLayers = true;
    #else
        constexpr const bool enableValidationLayers = false;
    #endif

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    constexpr const int MAX_FRAMES_IN_FLIGHT = 2;

    class Render_Core : public Singleton<Render_Core>
    {
        public:
            Render_Core() = default;

            void init();
            void destroy();

            inline Instance& getInstance() noexcept { return _instance; }
            inline Device& getDevice() noexcept { return _device; }
            inline Queues& getQueue() noexcept { return _queues; }
			inline ValidationLayers& getLayers() noexcept { return _layers; }

			~Render_Core() = default;

        private:
			ValidationLayers _layers;
            Queues _queues;
            Device _device;
            Instance _instance;
            bool _is_init = false;
    };
}

#endif // __MLX_RENDER_CORE__
