/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_keycodes.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 00:15:49 by rprieur           #+#    #+#             */
/*   Updated: 2026/08/09 00:16:30 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// MacroLibX official repo https://github.com/seekrs/MacroLibX
// MacroLibX official website https://macrolibx.kbz8.me/

#ifndef MACROLIB_X_KEYCODES_H
#define MACROLIB_X_KEYCODES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief           Event codes for physical keyboard keys
 */
typedef enum mlx_key_event_code
{
	MLX_KEY_A				= 4,
	MLX_KEY_B				= 5,
	MLX_KEY_C				= 6,
	MLX_KEY_D				= 7,
	MLX_KEY_E				= 8,
	MLX_KEY_F				= 9,
	MLX_KEY_G				= 10,
	MLX_KEY_H				= 11,
	MLX_KEY_I				= 12,
	MLX_KEY_J				= 13,
	MLX_KEY_K				= 14,
	MLX_KEY_L				= 15,
	MLX_KEY_M				= 16,
	MLX_KEY_N				= 17,
	MLX_KEY_O				= 18,
	MLX_KEY_P				= 19,
	MLX_KEY_Q				= 20,
	MLX_KEY_R				= 21,
	MLX_KEY_S				= 22,
	MLX_KEY_T				= 23,
	MLX_KEY_U				= 24,
	MLX_KEY_V				= 25,
	MLX_KEY_W				= 26,
	MLX_KEY_X				= 27,
	MLX_KEY_Y				= 28,
	MLX_KEY_Z				= 29,
	MLX_KEY_1				= 30,
	MLX_KEY_2				= 31,
	MLX_KEY_3				= 32,
	MLX_KEY_4				= 33,
	MLX_KEY_5				= 34,
	MLX_KEY_6				= 35,
	MLX_KEY_7				= 36,
	MLX_KEY_8				= 37,
	MLX_KEY_9				= 38,
	MLX_KEY_0				= 39,
	MLX_KEY_RETURN			= 40,
	MLX_KEY_ENTER			= 40,
	MLX_KEY_ESCAPE			= 41,
	MLX_KEY_BACKSPACE		= 42,
	MLX_KEY_TAB				= 43,
	MLX_KEY_SPACE			= 44,
	MLX_KEY_MINUS			= 45,
	MLX_KEY_EQUALS			= 46,
	MLX_KEY_LEFTBRACKET		= 47,
	MLX_KEY_RIGHTBRACKET	= 48,
	MLX_KEY_BACKSLASH		= 49,
	MLX_KEY_SEMICOLON		= 51,
	MLX_KEY_APOSTROPHE		= 52,
	MLX_KEY_GRAVE			= 53,
	MLX_KEY_COMMA			= 54,
	MLX_KEY_PERIOD			= 55,
	MLX_KEY_SLASH			= 56,
	MLX_KEY_CAPSLOCK		= 57,
	MLX_KEY_F1				= 58,
	MLX_KEY_F2				= 59,
	MLX_KEY_F3				= 60,
	MLX_KEY_F4				= 61,
	MLX_KEY_F5				= 62,
	MLX_KEY_F6				= 63,
	MLX_KEY_F7				= 64,
	MLX_KEY_F8				= 65,
	MLX_KEY_F9				= 66,
	MLX_KEY_F10				= 67,
	MLX_KEY_F11				= 68,
	MLX_KEY_F12				= 69,
	MLX_KEY_PRINTSCREEN		= 70,
	MLX_KEY_SCROLLLOCK		= 71,
	MLX_KEY_PAUSE			= 72,
	MLX_KEY_INSERT			= 73,
	MLX_KEY_HOME			= 74,
	MLX_KEY_PAGEUP			= 75,
	MLX_KEY_DELETE			= 76,
	MLX_KEY_END				= 77,
	MLX_KEY_PAGEDOWN		= 78,
	MLX_KEY_RIGHT			= 79,
	MLX_KEY_LEFT			= 80,
	MLX_KEY_DOWN			= 81,
	MLX_KEY_UP				= 82,
	MLX_KEY_NUMLOCKCLEAR	= 83,
	MLX_KEY_KP_DIVIDE		= 84,
	MLX_KEY_KP_MULTIPLY		= 85,
	MLX_KEY_KP_MINUS		= 86,
	MLX_KEY_KP_PLUS			= 87,
	MLX_KEY_KP_ENTER		= 88,
	MLX_KEY_KP_1			= 89,
	MLX_KEY_KP_2			= 90,
	MLX_KEY_KP_3			= 91,
	MLX_KEY_KP_4			= 92,
	MLX_KEY_KP_5			= 93,
	MLX_KEY_KP_6			= 94,
	MLX_KEY_KP_7			= 95,
	MLX_KEY_KP_8			= 96,
	MLX_KEY_KP_9			= 97,
	MLX_KEY_KP_0			= 98,
	MLX_KEY_KP_PERIOD		= 99,
	MLX_KEY_LCTRL			= 224,
	MLX_KEY_LSHIFT			= 225,
	MLX_KEY_LALT			= 226,
	MLX_KEY_LGUI			= 227,
	MLX_KEY_RCTRL			= 228,
	MLX_KEY_RSHIFT			= 229,
	MLX_KEY_RALT			= 230,
	MLX_KEY_RGUI			= 231
} mlx_key_codes;

/**
 * @brief           Event codes for mouse buttons
 */
typedef enum mlx_mouse_code
{
	MLX_MOUSE_LEFT			= 1,
	MLX_MOUSE_MIDDLE		= 2,
	MLX_MOUSE_RIGHT			= 3,
	MLX_MOUSE_4				= 4,
	MLX_MOUSE_5				= 5
} mlx_mouse_code;

/**
 * @brief           Event codes for mouse scrolling
 */
typedef enum mlx_mouse_scroll_code
{
	MLX_MOUSE_SCROLL_UP			= 1,
	MLX_MOUSE_SCROLL_DOWN		= 2,
	MLX_MOUSE_SCROLL_RIGHT		= 3,
	MLX_MOUSE_SCROLL_LEFT		= 4
} mlx_mouse_scroll_code;

/**
 * @brief           Event codes for controller buttons
 */
typedef enum mlx_controller_code
{
	MLX_CONTROLLER_A				= 0,
    MLX_CONTROLLER_B				= 1,
    MLX_CONTROLLER_X				= 2,
    MLX_CONTROLLER_Y				= 3,
    MLX_CONTROLLER_BACK				= 4,
    MLX_CONTROLLER_GUIDE			= 5,
    MLX_CONTROLLER_START			= 6,
    MLX_CONTROLLER_LEFTSTICK		= 7,
    MLX_CONTROLLER_RIGHTSTICK		= 8,
    MLX_CONTROLLER_LEFTSHOULDER		= 9,
    MLX_CONTROLLER_RIGHTSHOULDER	= 10,
    MLX_CONTROLLER_DPAD_UP			= 11,
    MLX_CONTROLLER_DPAD_DOWN		= 12,
    MLX_CONTROLLER_DPAD_LEFT		= 13,
    MLX_CONTROLLER_DPAD_RIGHT		= 14
} mlx_controller_code;

/**
 * @brief           Event codes for window events
 */
typedef enum mlx_window_code
{
	MLX_WINDOW_CLOSE		= 0,
	MLX_WINDOW_MOVED		= 1,
	MLX_WINDOW_MINIMIZED	= 2,
	MLX_WINDOW_MAXIMIZED	= 3,
	MLX_WINDOW_ENTER		= 4,
	MLX_WINDOW_FOCUS		= 5,
	MLX_WINDOW_LEAVE		= 6,
	MLX_WINDOW_UNFOCUS		= 7,
	MLX_WINDOW_SIZE_CHANGED	= 8,
	MLX_WINDOW_RESIZED		= 9,
	MLX_WINDOW_RESTORED		= 11
} mlx_window_code;

#ifdef __cplusplus
}
#endif

#endif
