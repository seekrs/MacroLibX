"""
macrolibpy — thin, Pythonic bindings over MacroLibX.
"""

from __future__ import annotations
from enum import IntEnum
from typing import Callable, Optional, Any, List
import os, sys
from cffi import FFI
import pathlib

__all__ = ["Context", "Window", "Image", "Event"]

ffi = FFI()

# NOTE: This is a curated declaration set focused on the core API
ffi.cdef(
    r"""
    typedef unsigned char uint8_t;
    typedef unsigned int uint32_t;

    typedef void (*mlx_function)(void);
    typedef struct mlx_context_handler* mlx_context;
    typedef struct mlx_window_handler* mlx_window;
    typedef struct mlx_image_handler* mlx_image;

    typedef struct mlx_color
    {
        uint32_t rgba;
    } mlx_color;

    typedef enum mlx_event_type
    {
        MLX_KEYDOWN = 0,
        MLX_KEYUP = 1,
        MLX_MOUSEDOWN = 2,
        MLX_MOUSEUP = 3,
        MLX_MOUSEWHEEL = 4,
        MLX_WINDOW_EVENT = 5
    } mlx_event_type;

    typedef struct mlx_window_create_info
    {
        mlx_image render_target;
        const char* title;
        int width;
        int height;
        _Bool is_fullscreen;
        _Bool is_resizable;
    } mlx_window_create_info;

    mlx_context mlx_init();
    void mlx_set_fps_goal(mlx_context mlx, int fps);
    void mlx_destroy_context(mlx_context mlx);
    mlx_window mlx_new_window(mlx_context mlx, const mlx_window_create_info* info);
    void mlx_destroy_window(mlx_context mlx, mlx_window win);
    void mlx_set_window_position(mlx_context mlx, mlx_window win, int x, int y);
    void mlx_set_window_size(mlx_context mlx, mlx_window win, int width, int height);
    void mlx_set_window_title(mlx_context mlx, mlx_window win, const char* title);
    void mlx_set_window_fullscreen(mlx_context mlx, mlx_window win, _Bool enable);
    void mlx_get_window_position(mlx_context mlx, mlx_window win, int* x, int* y);
    void mlx_get_window_size(mlx_context mlx, mlx_window win, int* x, int* y);
    void mlx_clear_window(mlx_context mlx, mlx_window win, mlx_color color);
    void mlx_get_screen_size(mlx_context mlx, mlx_window win, int* w, int* h);
    void mlx_add_loop_hook(mlx_context mlx, void(*f)(void*), void* param);
    void mlx_loop(mlx_context mlx);
    void mlx_loop_end(mlx_context mlx);
    void mlx_mouse_show(mlx_context mlx);
    void mlx_mouse_hide(mlx_context mlx);
    void mlx_mouse_move(mlx_context mlx, mlx_window win, int x, int y);
    void mlx_mouse_get_pos(mlx_context mlx, int* x, int* y);
    void mlx_on_event(mlx_context mlx, mlx_window win, mlx_event_type event, void(*f)(int, void*), void* param);
    void mlx_pixel_put(mlx_context mlx, mlx_window win, int x, int y, mlx_color color);
    mlx_image mlx_new_image(mlx_context mlx, int width, int height);
    mlx_image mlx_new_image_from_file(mlx_context mlx, char* filename, int* width, int* height);
    void mlx_destroy_image(mlx_context mlx, mlx_image image);
    mlx_color mlx_get_image_pixel(mlx_context mlx, mlx_image image, int x, int y);
    void mlx_set_image_pixel(mlx_context mlx, mlx_image image, int x, int y, mlx_color color);
    void mlx_put_image_to_window(mlx_context mlx, mlx_window win, mlx_image image, int x, int y);
    void mlx_string_put(mlx_context mlx, mlx_window win, int x, int y, mlx_color color, char* str);
    void mlx_set_font(mlx_context mlx, char* filepath);
    void mlx_set_font_scale(mlx_context mlx, char* filepath, float scale);
    """
)

def _candidateLibNames() -> List[str]:
    if sys.platform.startswith("linux"):
        return ["libmlx.so"]
    if sys.platform == "darwin":
        return ["libmlx.dylib"]
    if sys.platform.startswith("win"):
        # depending on build system, either of these may exist
        return ["mlx.dll", "libmlx.dll"]
    return []

def _candidateSdl2Names() -> List[str]:
    if sys.platform.startswith("linux"):
        # Try common SONAMEs (distro dependent)
        return ["libSDL2-2.0.so.0", "libSDL2.so.0", "libSDL2.so"]
    if sys.platform == "darwin":
        return ["libSDL2-2.0.0.dylib", "libSDL2.dylib"]
    if sys.platform.startswith("win"):
        return ["SDL2.dll"]
    return []

def _loadLibrary(candidates: List[str]):
    last_err: Optional[BaseException] = None
    RTLD_NOW = 2
    RTLD_GLOBAL = 0x100
    for path in candidates:
        try:
            return ffi.dlopen(path, RTLD_NOW | RTLD_GLOBAL)
        except OSError as e:
            last_err = e
    raise OSError(
        f"Could not load library:\n"
        f"\tTried: {candidates}.\n"
        f"\tLast error: {last_err}"
    )

def _dlopenMlx():
    _loadLibrary(_candidateSdl2Names())
    candidates: List[str] = []
    for name in _candidateLibNames():
        candidates.append(os.path.join(pathlib.Path(__file__).parent.parent.resolve(), name))
    return _loadLibrary(candidates)

lib = _dlopenMlx()

class EventType(IntEnum):
    KEYDOWN = 0
    KEYUP = 1
    MOUSEDOWN = 2
    MOUSEUP = 3
    MOUSEWHEEL = 4
    WINDOW_EVENT = 5

class MlxError(RuntimeError):
    pass

# Keep Python references to cffi callbacks alive
_alive_callbacks: List[Any] = []

def _makeLoopCallback(pyfunc: Callable[[], None]):
    @ffi.callback("void(void*)")
    def cfunc(userdata):
        pyfunc()
    _alive_callbacks.append(cfunc)
    return cfunc

def _makeEventCallback(pyfunc: Callable[[int], None]):
    @ffi.callback("void(int, void*)")
    def cfunc(event, userdata):
        pyfunc(int(event))
    _alive_callbacks.append(cfunc)
    return cfunc

def _rgbaToColor(rgba: int):
    c = ffi.new("mlx_color*")
    c[0].rgba = int(rgba) & 0xFFFFFFFF
    return c[0]

class Context:
    __slots__ = ("_ctx",)

    def __init__(self, _ptr) -> None:
        self._ctx = _ptr

    @classmethod
    def create(cls) -> "Context":
        ctx = lib.mlx_init()
        if ctx == ffi.NULL:
            raise MlxError("mlx_init failed")
        return cls(ctx)

    def set_fps_goal(self, fps: int) -> None:
        lib.mlx_set_fps_goal(self._ctx, int(fps))

    def add_loop_hook(self, fn: Callable[[], None]) -> None:
        lib.mlx_add_loop_hook(self._ctx, _makeLoopCallback(fn), ffi.NULL)

    def new_window(
        self,
        width: int,
        height: int,
        title: str,
        resizable: bool = True,
        fullscreen: bool = False,
    ) -> "Window":
        info = ffi.new("mlx_window_create_info*")
        info.render_target = ffi.NULL
        info.title = ffi.from_buffer(title.encode("utf-8"))
        info.width = int(width)
        info.height = int(height)
        info.is_fullscreen = bool(fullscreen)
        info.is_resizable = bool(resizable)
        win = lib.mlx_new_window(self._ctx, info)
        if win == ffi.NULL:
            raise MlxError("mlx_new_window failed")
        return Window(self, win)

    def new_image(self, width: int, height: int) -> "Image":
        img = lib.mlx_new_image(self._ctx, int(width), int(height))
        if img == ffi.NULL:
            raise MlxError("mlx_new_image failed")
        return Image(self, img)

    def loop(self) -> None:
        lib.mlx_loop(self._ctx)

    def loop_end(self) -> None:
        lib.mlx_loop_end(self._ctx)

    def destroy(self) -> None:
        if getattr(self, "_ctx", None):
            lib.mlx_destroy_context(self._ctx)
            self._ctx = ffi.NULL

    def __del__(self):
        try:
            self.destroy()
        except Exception:
            pass

class Window:
    __slots__ = ("_ctx", "_win")

    def __init__(self, ctx: Context, win) -> None:
        self._ctx = ctx
        self._win = win

    def set_title(self, title: str) -> None:
        lib.mlx_set_window_title(self._ctx._ctx, self._win, ffi.from_buffer(title.encode("utf-8")))

    def clear(self, rgba: int) -> None:
        lib.mlx_clear_window(self._ctx._ctx, self._win, _rgbaToColor(rgba))

    def pixel_put(self, x: int, y: int, rgba: int) -> None:
        lib.mlx_pixel_put(self._ctx._ctx, self._win, int(x), int(y), _rgbaToColor(rgba))

    def put_image(self, image: "Image", x: int = 0, y: int = 0) -> None:
        lib.mlx_put_image_to_window(self._ctx._ctx, self._win, image._img, int(x), int(y))

    def on_event(self, event_type: EventType | int, fn: Callable[[int], None]) -> None:
        lib.mlx_on_event(self._ctx._ctx, self._win, int(event_type), _makeEventCallback(fn), ffi.NULL)

    def destroy(self) -> None:
        if getattr(self, "_win", None):
            lib.mlx_destroy_window(self._ctx._ctx, self._win)
            self._win = ffi.NULL

    def __del__(self):
        try:
            self.destroy()
        except Exception:
            pass

class Image:
    __slots__ = ("_ctx", "_img")

    def __init__(self, ctx: Context, img) -> None:
        self._ctx = ctx
        self._img = img

    def destroy(self) -> None:
        if getattr(self, "_img", None):
            lib.mlx_destroy_image(self._ctx._ctx, self._img)
            self._img = ffi.NULL

    def __del__(self):
        try:
            self.destroy()
        except Exception:
            pass
