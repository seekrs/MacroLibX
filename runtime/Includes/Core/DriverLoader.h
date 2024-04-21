/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DriverLoader.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:56:10 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/03 15:02:44 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_CORE_DRIVER_LOADER__
#define __MLX_CORE_DRIVER_LOADER__

#include <Drivers/DriverInstance.h>

namespace mlx
{
	class DriverLoader
	{
		public:
			DriverLoader() = default;

			template <typename T>
			inline bool LoadDriver();

			inline void ShutdownAllDrivers();

			~DriverLoader() = default;

		private:
			std::vector<std::unique_ptr<DriverInstance> > m_instances;
	};
}

#include <Core/DriverLoader.inl>

#endif
