project('MacroLibX',
  ['c', 'cpp'],
  version : '2.2.4',
  license : 'MIT',
   meson_version : '>= 1.9.0',
  default_options : ['warning_level=2', 'optimization=3', 'cpp_std=c++20'])

add_project_arguments('-Wno-error=', language : 'c')
add_project_arguments('-fPIC', language : 'c')
add_project_arguments('-DSDL_MAIN_HANDLED', language : 'c')

if get_option('graphics_memory_dump')
  add_project_arguments('-DGRAPHICS_MEMORY_DUMP', language : 'c')
endif

if get_option('profiler')
  add_project_arguments('-DPROFILER_ENABLED', language : 'c')
endif

if get_option('force_wayland')
  add_project_arguments('-DFORCE_WAYLAND', language : 'c')
endif

if get_option('disable_all_safeties')
  add_project_arguments('-DDISABLE_ALL_SAFETIES', language : 'c')
endif

includes_directories = [
  include_directories('includes'),
  include_directories('runtime/Includes'),
  include_directories('runtime/Sources'),
  include_directories('third_party'),
]

sources = [
  files(
    'runtime/Sources/Core/Application.cpp',
    'runtime/Sources/Core/Bridge.cpp',
    'runtime/Sources/Core/EventBus.cpp',
    'runtime/Sources/Core/EventListener.cpp',
    'runtime/Sources/Core/Fps.cpp',
    'runtime/Sources/Core/Graphics.cpp',
    'runtime/Sources/Core/Logs.cpp',
    'runtime/Sources/Core/Memory.cpp',
    'runtime/Sources/Core/Profiler.cpp',
    'runtime/Sources/Core/SDLManager.cpp',
    'runtime/Sources/Core/UUID.cpp',
    'runtime/Sources/Graphics/Font.cpp',
    'runtime/Sources/Graphics/Mesh.cpp',
    'runtime/Sources/Graphics/PutPixelManager.cpp',
    'runtime/Sources/Graphics/Scene.cpp',
    'runtime/Sources/Graphics/Sprite.cpp',
    'runtime/Sources/Graphics/Text.cpp',
    'runtime/Sources/Platform/Inputs.cpp',
    'runtime/Sources/Platform/Window.cpp',
    'runtime/Sources/Renderer/Pipelines/Graphics.cpp',
    'runtime/Sources/Renderer/Pipelines/Shader.cpp',
    'runtime/Sources/Renderer/RenderPasses/2DPass.cpp',
    'runtime/Sources/Renderer/RenderPasses/FinalPass.cpp',
    'runtime/Sources/Renderer/RenderPasses/Passes.cpp',
    'runtime/Sources/Renderer/Vulkan/VulkanLoader.cpp',
    'runtime/Sources/Renderer/Buffer.cpp',
    'runtime/Sources/Renderer/Descriptor.cpp',
    'runtime/Sources/Renderer/Image.cpp',
    'runtime/Sources/Renderer/Memory.cpp',
    'runtime/Sources/Renderer/RenderCore.cpp',
    'runtime/Sources/Renderer/Renderer.cpp',
    'runtime/Sources/Renderer/SceneRenderer.cpp',
    'runtime/Sources/Renderer/Swapchain.cpp'
  )
]

mlx_headers = [
  'includes/mlx.h',
  'includes/mlx_extended.h',
  'includes/mlx_profile.h'
]

install_headers(mlx_headers)

deps = [
  dependency('sdl2'),
]

libmlx = library('mlx',
  sources,
  include_directories : includes_directories,
  dependencies : deps,
  install : true)

libmlx_dep = declare_dependency(
  include_directories: includes_directories,
  dependencies : deps,
  link_with : libmlx)

meson.override_dependency('mlx', libmlx_dep)

# PKG CONFIG GENERATION
pkg_mod = import('pkgconfig')

pkg_mod.generate(
  libmlx,
  name : 'mlx',
  description : 'MacroLibX Library',
  version : meson.project_version(),
  subdirs : ['.'], 
)
