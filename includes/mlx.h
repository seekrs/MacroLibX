/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:56:35 by maldavid          #+#    #+#             */
/*   Updated: 2024/09/12 01:28:12 by tdelage          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// MacroLibX official repo https://github.com/seekrs/MacroLibX
// MacroLibX official website https://macrolibx.kbz8.me/

#ifndef __MACRO_LIB_X_H__
#define __MACRO_LIB_X_H__

#include "mlx_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	MLX_KEYDOWN = 0,
	MLX_KEYUP = 1,
	MLX_MOUSEDOWN = 2,
	MLX_MOUSEUP = 3,
	MLX_MOUSEWHEEL = 4,
	MLX_WINDOW_EVENT = 5
} mlx_event_type;


/**
 * @brief			Initializes the MLX internal application
 *
 * @return (void*)	An opaque pointer to the internal MLX application or NULL (0x0) in case of error
 */
MLX_API void* mlx_init();


/**
 * @brief			Creates a new window
 *
 * @param mlx		Internal MLX application
 * @param w			Width of the window
 * @param h			Height of the window
 * @param title		Title of the window
 *
 * @return (void*)	An opaque pointer to the internal MLX window or NULL (0x0) in case of error
 */
MLX_API void* mlx_new_window(void* mlx, int w, int h, const char* title);

/**
 * @brief			Creates a new window
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window to move
 * @param x			New x position
 * @param y                     New y position
 *
 */
MLX_API void mlx_set_window_position(void *mlx, void *win, int x, int y);


/**
 * @brief			Gives a function to be executed at each loop turn
 *
 * @param mlx		Internal MLX application
 * @param f			The function
 * @param param		Param to give to the function passed
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_loop_hook(void* mlx, int (*f)(void*), void* param);


/**
 * @brief			Starts the internal main loop
 *
 * @param mlx		Internal MLX application
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_loop(void* mlx);


/**
 * @brief			Ends the internal main loop
 *
 * @param mlx		Internal MLX application
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_loop_end(void* mlx);


/**
 * @brief			Shows mouse cursor
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_mouse_show();


/**
 * @brief			Hides mouse cursor
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_mouse_hide();


/**
 * @brief			Moves cursor to given position
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window from which cursor moves
 * @param x			X coordinate
 * @param y			Y coordinate
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_mouse_move(void* mlx, void* win, int x, int y);


/**
 * @brief			Get cursor's position
 *
 * @param mlx		Internal MLX application
 * @param x			Get x coordinate
 * @param y			Get y coordinate
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_mouse_get_pos(void* mlx, int* x, int* y);


/**
 * @brief			Gives a function to be executed on event type
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param event		Event type (see union on top of this file)
 * @param f			Function to be executed
 * @param param		Parameter given to the function
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_on_event(void* mlx, void* win, mlx_event_type event, int (*f)(int, void*), void* param);


/**
 * @brief			Put a pixel in the window
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param x			X coordinate
 * @param y			Y coordinate
 * @param color		Color of the pixel (coded on 4 bytes in an int, 0xAARRGGBB)
 *
 * Note : If your're reading pixel colors from an image, don't forget to shift them 
 * one byte to the right as image pixels are encoded as 0xRRGGBBAA and pixel put takes 0xAARRGGBB.
 * 
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_pixel_put(void* mlx, void* win, int x, int y, int color);


/**
 * @brief			Create a new empty image
 *
 * @param mlx		Internal MLX application
 * @param width		Width of the image
 * @param height	Height of the image
 *
 * @return (void*)	An opaque pointer to the internal image or NULL (0x0) in case of error
 */
MLX_API void* mlx_new_image(void* mlx, int width, int height);


/**
 * @brief					Get image pixel data
 *
 * @param mlx				Internal MLX application
 * @param img				Internal image
 * @param x					X coordinate in the image
 * @param y					Y coordinate in the image
 *
 * @return (int)			Return the pixel data
 *
 * /!\ If you run into glitches when writing or reading pixels from images /!\
 * You need to add IMAGES_OPTIMIZED=false to your make mlx command
 * ```
 * 	~ git clone https://github.com/seekrs/MacroLibX.git
 * 	~ cd MacroLibX
 * 	~ make IMAGES_OPTIMIZED=false
 * ```
 */
MLX_API int mlx_get_image_pixel(void* mlx, void* img, int x, int y);


/**
 * @brief					Set image pixel data
 *
 * @param mlx				Internal MLX application
 * @param img				Internal image
 * @param x					X coordinate in the image
 * @param y					Y coordinate in the image
 * @param color				Color of the pixel to set
 *
 * @return (void)
 *
 * /!\ If you run into glitches when writing or reading pixels from images /!\
 * You need to add IMAGES_OPTIMIZED=false to your make mlx command
 * ```
 * 	~ git clone https://github.com/seekrs/MacroLibX.git
 * 	~ cd MacroLibX
 * 	~ make IMAGES_OPTIMIZED=false
 * ```
 */
MLX_API void mlx_set_image_pixel(void* mlx, void* img, int x, int y, int color);


/**
 * @brief			Put image to the given window
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param img		Internal image
 * @param x			X coordinate
 * @param y			Y coordinate
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_put_image_to_window(void* mlx, void* win, void* img, int x, int y);


/**
 * @brief			Destroys internal image
 *
 * @param mlx		Internal MLX application
 * @param img		Internal image
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_destroy_image(void* mlx, void* img);


/**
 * @brief			Create a new image from a png file
 *
 * @param mlx		Internal MLX application
 * @param filename	Path to the png file
 * @param width		Get the width of the image
 * @param heigth	Get the height of the image
 *
 * @return (void*)	An opaque pointer to the internal image or NULL (0x0) in case of error
 */
MLX_API void* mlx_png_file_to_image(void* mlx, char* filename, int* width, int* height);


/**
 * @brief			Create a new image from a jpg file
 *
 * @param mlx		Internal MLX application
 * @param filename	Path to the jpg file
 * @param width		Get the width of the image
 * @param heigth	Get the height of the image
 *
 * @return (void*)	An opaque pointer to the internal image or NULL (0x0) in case of error
 */
MLX_API void* mlx_jpg_file_to_image(void* mlx, char* filename, int* width, int* height);


/**
 * @brief			Create a new image from a bmp file
 *
 * @param mlx		Internal MLX application
 * @param filename	Path to the bmp file
 * @param width		Get the width of the image
 * @param heigth	Get the height of the image
 *
 * @return (void*)	An opaque pointer to the internal image or NULL (0x0) in case of error
 */
MLX_API void* mlx_bmp_file_to_image(void* mlx, char* filename, int* width, int* height);


/**
 * @brief			Put text in given window
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param x			X coordinate
 * @param y			Y coordinate
 * @param color		Color of the pixel (coded on 4 bytes in an int, 0xAARRGGBB)
 * @param str		Text to put
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_string_put(void* mlx, void* win, int x, int y, int color, char* str);


/**
 * @brief			Loads a font to be used by `mlx_string_put`
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param filepath	Filepath to the font or "default" to reset to the embedded font
 *
 * @return (void)	
 */
MLX_API void mlx_set_font(void* mlx, void* win, char* filepath);


/**
 * @brief			Loads a font to be used by `mlx_string_put` and scales it
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param filepath	Filepath to the font or "default" to reset to the embedded font
 * @param scale		Scale to apply to the font
 *
 * @return (void)	
 */
MLX_API void mlx_set_font_scale(void* mlx, void* win, char* filepath, float scale);


/**
 * @brief			Clears the given window (resets all rendered data)
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_clear_window(void* mlx, void* win);


/**
 * @brief			Destroys internal window
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_destroy_window(void* mlx, void* win);

/**
 * @brief			Destroy internal MLX application
 *
 * @param mlx		Internal MLX application
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_destroy_display(void* mlx);


/**
 * @brief			Get the size of the screen the given window is on
 *
 * @param mlx		Internal MLX application
 * @param win		Internal window
 * @param w			Get width size
 * @param h			Get height size
 *
 * @return (int)	Always return 0, made this to copy the behaviour of the original MLX
 */
MLX_API int mlx_get_screens_size(void* mlx, void* win, int* w, int* h);


/**
 * @brief			Caps the FPS
 *
 * @param mlx		Internal MLX application
 * @param fps		The FPS cap
 *
 * @return (int)	Always return 0
 */
MLX_API int mlx_set_fps_goal(void* mlx, int fps);

#ifdef __cplusplus
}
#endif

#endif
