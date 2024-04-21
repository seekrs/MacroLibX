/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DriverInstance.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:57:20 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/02 17:01:03 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_DRIVER_INSTANCE__
#define __MLX_DRIVER_INSTANCE__

namespace mlx
{
	class DriverInstance
	{
		public:
			DriverInstance() = default;

			virtual bool InitDriver() { m_is_up = true; return true; }
			virtual void ShutdownDriver() { m_is_up = false; }

			inline bool IsRunning() const noexcept { return m_is_up; }

			virtual ~DriverInstance() = default;

		private:
			bool m_is_up = false;
	};
}

#endif
