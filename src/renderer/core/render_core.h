/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:16:32 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 17:51:47 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

#include <volk.h>

#include "vk_queues.h"
#include "vk_device.h"
#include "vk_instance.h"

#include <utils/singleton.h>
#include <core/errors.h>

namespace mlx
{
    namespace RCore
    {
        void checkVk(VkResult result);
    }

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

			~Render_Core() = default;

        private:
            Queues _queues;
            Device _device;
            Instance _instance;
            bool _is_init = false;
    };
}

#endif // __MLX_RENDER_CORE__
