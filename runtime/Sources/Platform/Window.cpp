/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/26 23:03:59 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_compiled.h>

#include <platform/window.h>
#include <core/errors.h>
#include <utils/icon_mlx.h>

namespace mlx
{
	Window::Window(std::size_t w, std::size_t h, const std::string& title) : _width(w), _height(h)
	{
		static std::uint64_t ids = 0;

		if(title.find("vvaas") != std::string::npos)
			core::error::report(e_kind::message, "vvaas est mauvais");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_win = glfwCreateWindow(_width, _height, title.c_str(), NULL, NULL);;
		_id = ids++;
	}

	void Window::destroy() noexcept
	{
		if(_win != nullptr)
		{
			glfwDestroyWindow(_win);
			_win = nullptr;
		}
	}
}
