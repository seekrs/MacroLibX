/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:56:35 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/24 17:52:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MACRO_LIB_X_H__
#define __MACRO_LIB_X_H__

#ifdef __cplusplus
extern "C" {
#endif

void* mlx_init();
void* mlx_new_window(void* mlx, int w, int h, const char* title);

int	mlx_loop_hook(void* mlx, int (*f)(void*), void* param);
int mlx_loop(void* mlx);
int mlx_loop_end(void* mlx);

int mlx_mouse_show();
int mlx_mouse_hide();
int mlx_mouse_move(void* win_ptr, int x, int y);
int mlx_mouse_get_pos(void* win_ptr, int* x, int* y);

int mlx_pixel_put(void* mlx, void* win_ptr, int x, int y, int color);

int mlx_destroy_window(void* mlx, void* win_ptr);
int mlx_destroy_display(void* mlx);

#ifdef __cplusplus
}
#endif

#endif
