/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/05 19:25:46 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/mlx.h"

int	main(void)
{
	void	*win_ptr;
	int		x;
	int		y;
	
	mlx_init();
	win_ptr = mlx_new_window(400, 400, "My window");
	mlx_mouse_get_pos(win_ptr, &x, &y);
	printf("%d, %d\n", x, y);
	mlx_loop();
	return (0);
}
