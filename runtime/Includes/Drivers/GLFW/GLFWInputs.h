/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWInputs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:32:29 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 18:37:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_GLFW_INPUTS__
#define __MLX_GLFW_INPUTS__

#include <Platform/Inputs.h>

namespace mlx
{
	class GLFWInputs : public Inputs
	{
		public:
			GLFWInputs() = default;

			void Update() override noexcept;

			void RegisterWindow(std::shared_ptr<Window> window) override;

			inline std::int32_t GetX() override const noexcept { return m_x; }
			inline std::int32_t GetY() override const noexcept { return m_y; }
			inline std::int32_t GetXRel() override const noexcept { return m_x_rel; }
			inline std::int32_t GetYRel() override const noexcept { return m_y_rel; }

			~GLFWInputs() override = default;

		private:
			std::int32_t m_x = 0;
			std::int32_t m_y = 0;
			std::int32_t m_x_rel = 0;
			std::int32_t m_y_rel = 0;
	};
}

#endif
