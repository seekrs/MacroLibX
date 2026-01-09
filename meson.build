project('MacroLibX',
  ['c', 'cpp'],
  version : '2.2.2',
  license : 'MIT',
   meson_version : '>= 1.9.0',
  default_options : ['warning_level=2', 'optimization=3', 'cpp_std=c++20'])

add_project_arguments('-Wno-error=', language : 'c')
add_project_arguments('-fPIC', language : 'c')
add_project_arguments('-DSDL_MAIN_HANDLED', language : 'c')

if get_option('force_integrated_gpu')
  add_project_arguments('-DFORCE_INTEGRATED_GPU', language : 'c')
endif

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

subdir('runtime/Sources')

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

# PKG CONFIG GENERATION
pkg_mod = import('pkgconfig')

pkg_mod.generate(
  libmlx,
  name : 'mlx',
  description : 'MacroLibX Library',
  version : meson.project_version(),
  subdirs : ['.'], 
)