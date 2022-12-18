/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:48:06 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/17 23:28:22 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <utility>

#include "errors.h"

namespace mlx::core::error
{
	void report(e_kind kind, std::string msg, ...)
	{
		char buffer[1024];

		va_list al;
		va_start(al, msg);
		std::vsprintf(buffer, std::move(msg).c_str(), al);
		va_end(al);

		switch(kind)
		{
			case e_kind::message: std::cout << "[MicroLibX] Message : " << buffer << std::endl; break;
			case e_kind::warning: std::cout << "[MicroLibX] Warning : " << buffer << std::endl; break;
			case e_kind::error: std::cerr << "[MicroLibX] Error : " << buffer << std::endl; break;
			case e_kind::fatal_error:
				std::cerr << "[MicroLibX] Fatal Error : " << buffer << std::endl;
				std::exit(EXIT_SUCCESS);
			break;
		}
	}
}
