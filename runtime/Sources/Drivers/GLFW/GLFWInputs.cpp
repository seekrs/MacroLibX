/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWInputs.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:39:32 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 18:42:18 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>
#include <Drivers/GLFW/GLFWInputs.h>

namespace mlx
{
	void GLFWInputs::Update() noexcept
	{
		glfwPollEvents();
	}
}
