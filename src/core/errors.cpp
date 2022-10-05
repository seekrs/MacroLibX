/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:48:06 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/04 21:18:35 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>

#include "errors.h"

namespace mlx::core::error
{
	void report(e_kind kind, std::string msg)
	{
		switch(kind)
		{
			case e_kind::message: std::cout << "[MicroLibX] Message : " << msg << std::endl; break;
			case e_kind::warning: std::cout << "[MicroLibX] Warning : " << msg << std::endl; break;
			case e_kind::error: std::cerr << "[MicroLibX] Error : " << msg << std::endl; break;
			case e_kind::fatal_error:
				std::cerr << "[MicroLibX] Fatal Error : " << msg << std::endl;
				std::exit(EXIT_SUCCESS);
			break;
		}
	}
}
