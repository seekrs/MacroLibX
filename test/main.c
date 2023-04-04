/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/04 14:54:04 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/mlx.h"

static char * exemple_xpm[] = {
"24 20 3 1",
" 	c None",
".	c #3A32E4",
"+	c #E43232",
"                        ",
"    ..                  ",
"   ....                 ",
"  ......++++++++        ",
" .........+++++++       ",
" ..........+++++++      ",
" ............++++++     ",
" .............++++++    ",
"  ..............++++    ",
"   +.............+++    ",
"   ++.............++    ",
"   +++.............+    ",
"   +++++.............   ",
"   ++++++.............. ",
"   ++++++++............ ",
"   +++++++++........... ",
"    +++++++++.........  ",
"     ++++++++++.......  ",
"      ++++++++++.....   ",
"       +++++++++ ...    "};

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*logo;
}	t_mlx;

int	update(t_mlx *mlx)
{
	static int	i = 0;
	int			j;

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo, 100, 100);
	j = 0;
	while (j < 400)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, 0xFF0000FF);
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, 0xFF0000FF);
		j++;
	}
	i++;
	if (i == 5000)
		mlx_clear_window(mlx->mlx, mlx->win);
	if (i > 10000)
		mlx_loop_end(mlx->mlx);
	return (0);
}

void	*create_image(t_mlx *mlx)
{
	int		ignore[3];
	void	*img;
	char	*addr;
	int		i;

	img = mlx_new_image(mlx->mlx, 100, 100);
	addr = mlx_get_data_addr(mlx->mlx, img, &ignore[0], &ignore[1], &ignore[2]);
	i = 0;
	while (i < (100 * 100) * 4)
	{
		if (i < 10000 || i > 20000)
		{
			addr[i + 0] = 0xFF;
			addr[i + 1] = i;
			addr[i + 2] = 0x00;
			addr[i + 3] = 0xFF;
		}
		i += 4;
	}
	return (img);
}

int	main(void)
{
	t_mlx	mlx;
	int		w;
	int		h;
	void	*img;
	void	*pic;

	mlx.mlx = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx, 400, 400, "My window");
	mlx.logo = mlx_png_file_to_image(mlx.mlx, "42_logo.png", &w, &h);
	pic = mlx_xpm_to_image(mlx.mlx, exemple_xpm, &w, &h);
	mlx_put_image_to_window(mlx.mlx, mlx.win, pic, 20, 20);
	mlx_pixel_put(mlx.mlx, mlx.win, 200, 10, 0xFFFF00FF);
	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.logo, 200, 200);
	img = create_image(&mlx);
	mlx_put_image_to_window(mlx.mlx, mlx.win, img, 200, 20);
	mlx_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);
	mlx_destroy_image(mlx.mlx, img);
	mlx_destroy_image(mlx.mlx, pic);
	mlx_destroy_image(mlx.mlx, mlx.logo);
	mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	return (0);
}
