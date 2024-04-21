/* **************************************************************************** */
/*                                                                              */
/*                                                         :::      ::::::::    */
/*    GLFWDriverInstance.inl                             :+:      :+:    :+:    */
/*                                                     +:+ +:+         +:+      */
/*    By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+         */
/*                                                 +#+#+#+#+#+   +#+            */
/*    Created: 2024/04/02 17:04:23 by maldavid          #+#    #+#              */
/*    Updated: 2024/04/02 17:04:23 by maldavid         ###   ########.fr        */
/*                                                                              */
/* **************************************************************************** */

#include <Drivers/GLFW/GLFWDriverInstance.h>

namespace mlx
{
	bool GLFWDriverInstance::InitDriver()
	{
		glfwSetErrorCallback([]([[maybe_unused]] int code, const char* desc)
		{
			FatalError("GLFW Driver Error : %", desc);
		});
		glfwInit();
		DebugLog("GLFW Driver loaded");
	}

	void GLFWDriverInstance::ShutdownDriver()
	{
		glfwTerminate();
		DebugLog("GLFW Driver shutted down");
	}
}
