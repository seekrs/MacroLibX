# MacroLibX

[![linux clang workflow](https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_clang.yml/badge.svg)](https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_clang.yml)
[![linux gcc workflow](https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_gcc.yml/badge.svg)](https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_gcc.yml)
[![macos workflow](https://github.com/420verfl0w/MacroLibX/actions/workflows/macos_x86.yml/badge.svg)](https://github.com/420verfl0w/MacroLibX/actions/workflows/macos_x86.yml)

A rewrite of School 42's MiniLibX using SDL2 and Vulkan. The goal of this version is to give a light, fast and modern graphical tool while keeping the same API as the version currently used at 42.

# Installation

## Linux
Dependances :

For Ubuntu/Debian
```bash
~ sudo apt update
~ sudo apt install libsdl2-2.0-0 libsdl2-dev build-essential
```

For Arch based distros
```bash
~ sudo pacman -S sdl2
```

### MacOS
Dependances :
* [MoltenVK](https://github.com/KhronosGroup/MoltenVK/)
* SDL2 `brew install SDL2`

# Get MacroLibX

```bash
~ git clone https://github.com/420verfl0w/MacroLibX.git
~ cd MacroLibX
~ make
```

# Compile your project

```bash
clang myApp.c MacroLibX/libmlx.so -lSDL2
```

## /!\ If you run into glitches when writing or reading pixels from images /!\

You need to add `IMAGES_OPTIMIZED=false` to your make mlx command

```bash
~ git clone https://github.com/420verfl0w/MacroLibX.git
~ cd MacroLibX
~ make IMAGES_OPTIMIZED=false
```
