/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWDriverInstance.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:01:51 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/02 17:04:12 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_GLFW_DRIVER_INSTANCE__
#define __MLX_GLFW_DRIVER_INSTANCE__

#include <Drivers/DriverInstance.h>

namespace mlx
{
	class GLFWDriverInstance : public DriverInstance
	{
		public:
			GLFWDriverInstance() = default;

			inline bool InitDriver() override;
			inline void ShutdownDriver() override;

			~GLFWDriverInstance() override = default;
	};
}

#include <Drivers/GLFW/GLFWDriverInstance.inl>

#endif
