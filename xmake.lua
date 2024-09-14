-- Global settings

add_requires("libsdl", { configs = { sdlmain = false }})

add_rules("mode.debug", "mode.release")
set_languages("cxx20", "c99")

set_objectdir("objs/xmake/$(os)_$(arch)")
set_targetdir("./")

set_optimize("fastest")

-- Options

option("images_optimized")
	set_default(true)
	add_defines("IMAGE_OPTIMIZED")
option_end()

option("force_integrated_gpu")
	set_default(false)
	add_defines("FORCE_INTEGRATED_GPU")
option_end()

option("graphics_memory_dump")
	set_default(false)
	add_defines("GRAPHICS_MEMORY_DUMP")
option_end()

option("profiler")
	set_default(false)
	add_defines("PROFILER")
option_end()

option("force_wayland")
	set_default(false)
	add_defines("FORCE_WAYLAND")
option_end()

-- Targets

target("mlx")
	set_default(true)
	set_license("MIT")
	set_kind("shared")
	add_options("images_optimized")
	add_options("force_integrated_gpu")
	add_options("graphics_memory_dump")
	add_options("profiler")
	add_options("force_wayland")
	add_includedirs("runtime/Includes", "runtime/Sources", "includes", "third_party")

	set_pcxxheader("runtime/Sources/PreCompiled.h")

	add_defines("MLX_BUILD", "SDL_MAIN_HANDLED")

	add_files("runtime/Sources/**.cpp")

	add_packages("libsdl")

	if is_mode("debug") then
		add_defines("DEBUG")
	end
target_end() -- optional but I think the code is cleaner with this -- optional but I think the code is cleaner with this

target("Test")
	set_default(false)
	set_kind("binary")
	set_targetdir("example")

	add_linkdirs("./")

	add_deps("mlx")

	add_files("example/main.c")

	add_defines("SDL_MAIN_HANDLED")

	add_packages("libsdl")
target_end()
