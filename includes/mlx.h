/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <contact@kbz8.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:56:35 by maldavid          #+#    #+#             */
/*   Updated: 2025/01/05 22:44:22 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// MacroLibX official repo https://github.com/seekrs/MacroLibX
// MacroLibX official website https://macrolibx.kbz8.me/

#ifndef MACROLIB_X_H
#define MACROLIB_X_H

#include <stdbool.h>
#include "mlx_profile.h"

#ifdef __cplusplus
extern "C" {
#endif


        /* MLX types */

/**
 * @brief           Opaque handle that represents the MLX context
 */
MLX_DEFINE_HANDLE(mlx_context);

/**
 * @brief           Opaque handle that represents a MLX window
 */
MLX_DEFINE_HANDLE(mlx_window);

/**
 * @brief           Opaque handle that represents a MLX image
 */
MLX_DEFINE_HANDLE(mlx_image);

/**
 * @brief           Union representing RGBA color with access to each part as bytes
 */
typedef union mlx_color
{
	struct
	{
		#if MLX_BYTEORDER == MLX_LITTLE_ENDIAN
			uint8_t a;
			uint8_t b;
			uint8_t g;
			uint8_t r;
		#else
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		#endif
	};
	uint32_t rgba;
} mlx_color;



        /* MLX backend related functions */


/**
 * @brief           Initializes the MLX internal application
 *
 * @return (mlx_context) An opaque handler to the internal MLX application or NULL (0x0) in case of error
 */
MLX_API mlx_context mlx_init();

/**
 * @brief            Caps the FPS
 *
 * @param mlx        Internal MLX application
 * @param fps        The FPS cap or 0 for vsync
 */
MLX_API void mlx_set_fps_goal(mlx_context mlx, int fps);

/**
 * @brief            Destroy internal MLX application
 *
 * @param mlx        Internal MLX application
 */
MLX_API void mlx_destroy_context(mlx_context mlx);



        /* Window related functions */


/**
 * @brief           Descriptor structure for window creation
 *
 * Note: if a valid mlx_image is passed as render_target, this window will not be a real system window
 * and will rather act as a gate to use any draw function to draw directly on an image.
 *
 * Ex: you could use mlx_string_put or mlx_pixel_put to draw on a given image and then use this image
 * with mlx_put_image_to_window to render it on a real window.
 *
 * See experimental/RenderToTexture/main.c for a concrete example.
 */
typedef struct mlx_window_create_info
{
	mlx_image render_target;
	const char* title;
	int width;
	int height;
	bool is_fullscreen;
	bool is_resizable;
} mlx_window_create_info;

/**
 * @brief            Creates a new window
 *
 * @param mlx        Internal MLX application
 * @param info       Pointer to a descriptor structure
 *
 * @return (mlx_widnow) An opaque handler to the internal MLX window or NULL (0x0) in case of error
 */
MLX_API mlx_window mlx_new_window(mlx_context mlx, const mlx_window_create_info* info);

/**
 * @brief            Destroys internal window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 */
MLX_API void mlx_destroy_window(mlx_context mlx, mlx_window win);

/**
 * @brief            Sets window position
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param x          New x position
 * @param y          New y position
 */
MLX_API void mlx_set_window_position(mlx_context mlx, mlx_window win, int x, int y);

/**
 * @brief            Sets window size
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param width      New width
 * @param height     New height
 */
MLX_API void mlx_set_window_size(mlx_context mlx, mlx_window win, int width, int height);

/**
 * @brief            Sets window title
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param title      New title
 */
MLX_API void mlx_set_window_title(mlx_context mlx, mlx_window win, const char* title);

/**
 * @brief            Enables/Disables window fullscreen mode
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param enable     Switch or not to fullscreen
 */
MLX_API void mlx_set_window_fullscreen(mlx_context mlx, mlx_window win, bool enable);

/**
 * @brief            Gets window position
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param x          Pointers to get position of the window
 * @param y          Pointers to get position of the window
 */
MLX_API void mlx_get_window_position(mlx_context mlx, mlx_window win, int* x, int* y);

/**
 * @brief            Gets window size
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param x          Pointers to get size of the window
 * @param y          Pointers to get size of the window
 */
MLX_API void mlx_get_window_size(mlx_context mlx, mlx_window win, int* x, int* y);

/**
 * @brief            Clears the given window (resets all rendered data)
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 */
MLX_API void mlx_clear_window(mlx_context mlx, mlx_window win, mlx_color color);

/**
 * @brief            Get the size of the screen the given window is on
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to choose screen the window is on
 * @param w          Get width size
 * @param h          Get height size
 */
MLX_API void mlx_get_screen_size(mlx_context mlx, mlx_window win, int* w, int* h);



        /* Loop related functions */


/**
 * @brief            Gives another function to be executed at each loop turn
 *
 * @param mlx        Internal MLX application
 * @param f          The function
 * @param param      Param to give to the function passed
 */
MLX_API void mlx_add_loop_hook(mlx_context mlx, void(*f)(void*), void* param);

/**
 * @brief            Starts the internal main loop
 *
 * @param mlx        Internal MLX application
 */
MLX_API void mlx_loop(mlx_context mlx);

/**
 * @brief            Ends the internal run loop
 *
 * @param mlx        Internal MLX application
 */
MLX_API void mlx_loop_end(mlx_context mlx);



        /* Events related functions */


/**
 * @brief            Shows mouse cursor
 *
 * @param mlx        Internal MLX application
 */
MLX_API void mlx_mouse_show(mlx_context mlx);

/**
 * @brief            Hides mouse cursor
 *
 * @param mlx        Internal MLX application
 */
MLX_API void mlx_mouse_hide(mlx_context mlx);

/**
 * @brief            Moves cursor to givent position
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window from which cursor moves
 * @param x          X coordinate
 * @param y          Y coordinate
 */
MLX_API void mlx_mouse_move(mlx_context mlx, mlx_window win, int x, int y);

/**
 * @brief            Get cursor's position
 *
 * @param mlx        Internal MLX application
 * @param x          Get x coordinate
 * @param y          Get y coordinate
 */
MLX_API void mlx_mouse_get_pos(mlx_context mlx, int* x, int* y);

/**
 * @brief            Type of event
 */
typedef enum mlx_event_type
{
	MLX_KEYDOWN = 0,
	MLX_KEYUP = 1,
	MLX_MOUSEDOWN = 2,
	MLX_MOUSEUP = 3,
	MLX_MOUSEWHEEL = 4,
	MLX_WINDOW_EVENT = 5
} mlx_event_type;

/**
 * @brief            Gives a function to be executed on event type, does not override previous functions
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param event      Event type (see union on top of this file)
 * @param f          Function to be executed
 * @param param      Parameter given to the function
 */
MLX_API void mlx_on_event(mlx_context mlx, mlx_window win, mlx_event_type event, void(*f)(int, void*), void* param);



        /* Pixels drawing related functions */


/**
 * @brief            Put a pixel in the window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param x          X coordinate
 * @param y          Y coordinate
 * @param color      Color of the pixel
 */
MLX_API void mlx_pixel_put(mlx_context mlx, mlx_window win, int x, int y, mlx_color color);



        /* Images related functions */


/**
 * @brief            Create a new empty image
 *
 * @param mlx        Internal MLX application
 * @param width      Width of the image
 * @param height     Height of the image
 *
 * @return (mlx_image) An opaque handler to the internal image or NULL (0x0) in case of error
 */
MLX_API mlx_image mlx_new_image(mlx_context mlx, int width, int height);

/**
 * @brief            Create a new image from a png/jpg/bmp file
 *
 * @param mlx        Internal MLX application
 * @param filename   Path to the png file
 * @param width      Get the width of the image
 * @param heigth     Get the height of the image
 *
 * @return (mlx_image) An opaque handler to the internal image or NULL (0x0) in case of error
 */
MLX_API mlx_image mlx_new_image_from_file(mlx_context mlx, char* filename, int* width, int* height);

/**
 * @brief            Destroys internal image
 *
 * @param mlx        Internal MLX application
 * @param img        Internal image
 */
MLX_API void mlx_destroy_image(mlx_context mlx, mlx_image image);

/**
 * @brief            Get image pixel data
 *
 * @param mlx        Internal MLX application
 * @param img        Internal image
 * @param x          X coordinate in the image
 * @param y          Y coordinate in the image
 *
 * @return (int)     Return the pixel data
 *
 * /!\ If you run into glitches when writing or reading pixels from images /!\
 * You need to add IMAGES_OPTIMIZED=false to your make mlx command
 * ```
 *     ~ git clone https://github.com/seekrs/MacroLibX.git
 *     ~ cd MacroLibX
 *     ~ make IMAGES_OPTIMIZED=false
 * ```
 */
MLX_API mlx_color mlx_get_image_pixel(mlx_context mlx, mlx_image image, int x, int y);

/**
 * @brief            Set image pixel data
 *
 * @param mlx        Internal MLX application
 * @param img        Internal image
 * @param x          X coordinate in the image
 * @param y          Y coordinate in the image
 * @param color      Color of the pixel to set
 *
 * /!\ If you run into glitches when writing or reading pixels from images /!\
 * You need to add IMAGES_OPTIMIZED=false to your make mlx command
 * ```
 *     ~ git clone https://github.com/seekrs/MacroLibX.git
 *     ~ cd MacroLibX
 *     ~ make IMAGES_OPTIMIZED=false
 * ```
 */
MLX_API void mlx_set_image_pixel(mlx_context mlx, mlx_image image, int x, int y, mlx_color color);

/**
 * @brief            Put image to the given window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param img        Internal image
 * @param x          X coordinate
 * @param y          Y coordinate
 */
MLX_API void mlx_put_image_to_window(mlx_context mlx, mlx_window win, mlx_image image, int x, int y);



        /* Strings drawing related functions */


/**
 * @brief            Put text in given window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param x          X coordinate
 * @param y          Y coordinate
 * @param color      Color of the pixel
 * @param str        Text to put
 */
MLX_API void mlx_string_put(mlx_context mlx, mlx_window win, int x, int y, mlx_color color, char* str);

/**
 * @brief            Loads a font to be used by `mlx_string_put`
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param filepath   Filepath to the font or "default" to reset to the embedded font
 */
MLX_API void mlx_set_font(mlx_context mlx, char* filepath);

/**
 * @brief            Loads a font to be used by `mlx_string_put` and scales it
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param filepath   Filepath to the font or "default" to reset to the embedded font
 * @param scale      Scale to apply to the font
 */
MLX_API void mlx_set_font_scale(mlx_context mlx, char* filepath, float scale);

#ifdef __cplusplus
}
#endif

#endif
