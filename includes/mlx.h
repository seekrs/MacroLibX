/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:56:35 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/06 16:07:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MACRO_LIB_X_H__
#define __MACRO_LIB_X_H__

#ifdef __cplusplus
extern "C" {
#endif

void* mlx_init();
void* mlx_new_window(void* mlx, int w, int h, const char* title);

int mlx_loop_hook(void* mlx, int (*f)(), void* param);
int mlx_loop(void* mlx);
int mlx_loop_end(void* mlx);

int mlx_mouse_show();
int mlx_mouse_hide();
int mlx_mouse_move(void* mlx, void* win, int x, int y);
int mlx_mouse_get_pos(void* mlx, int* x, int* y);

int mlx_do_key_autorepeaton(void* mlx);
int mlx_do_key_autorepeatoff(void* mlx);

int mlx_pixel_put(void* mlx, void* win, int x, int y, int color);

void* mlx_new_image(void* mlx, int width, int height);
char* mlx_get_data_addr(void* mlx, void* img, int* bits_per_pixel, int* size_line, int* endian);
int mlx_put_image_to_window(void* mlx, void* win, void* img, int x, int y);
int mlx_destroy_image(void* mlx, void* img);

void* mlx_png_file_to_image(void* mlx, char* filename, int* width, int* height);
void* mlx_jpg_file_to_image(void* mlx, char* filename, int* width, int* height);
void* mlx_bmp_file_to_image(void* mlx, char* filename, int* width, int* height);

int mlx_string_put(void* mlx, void* win, int x, int y, int color, char* str);

int mlx_clear_window(void* mlx, void* win);

int mlx_destroy_window(void* mlx, void* win);
int mlx_destroy_display(void* mlx);

int mlx_get_screens_size(void* mlx, int* w, int* h);

#ifdef __cplusplus
}
#endif

#endif
