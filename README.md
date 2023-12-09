<div align="center">
    <img src="./res/logo.png" alt="drawing" width="200"/>
    <div align="center">
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_clang.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_clang.yml/badge.svg"></a>
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_gcc.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_gcc.yml/badge.svg"></a>
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/macos_x86.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/macos_x86.yml/badge.svg"></a>
    </div>
    <div align="center">
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/windows.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/windows.yml/badge.svg"></a>
    </div>
</div>

###### MacroLibX, a rewrite of 42 School's MiniLibX using SDL2 and Vulkan. 
The goal of this version is to provide a light, fast, and modern graphical tool while keeping the same API.

## 🖥️ Installation

### Dependencies
You first need to install the proper dependencies for your operating-system. 

#### 🐧 Linux
Here are a few common cases for different Linux distributions:

<details>
  <summary>
    For <a href="https://ubuntu.com">Ubuntu</a>/<a href="https://debian.org">Debian</a>-based distros:
  </summary>
<pre><code><!--
-->sudo apt update
sudo apt install libsdl2-2.0-0 libsdl2-dev build-essential
</code></pre>
</details>

<details>
  <summary>
    For <a href="https://archlinux.org">ArchLinux</a>-based distros:
  </summary>
<pre><code>sudo pacman -S sdl2</code></pre>
</details>

#### 🍎 macOS
[MacroLibX](#) on macOS requires [SDL2](#) and [MoltenVK](https://github.com/KhronosGroup/MoltenVK). You can install both using the [Homebrew](https://brew.sh) package manager:
```sh
brew install molten-vk
brew install SDL2
```

### 🪟 Windows
To build on Windows you may need to use the [xmake](https://xmake.io) build. [Here's](./XMAKE_BUILD.md) how you can use it.

### Clone and Build
Finally, you can clone the Git repository. When inside it, run the GNU `make` command to compile MacroLibX. 
```bash
git clone https://github.com/420verfl0w/MacroLibX.git
cd MacroLibX
make
```

## 🔨 Compile your project
To compile your project with MacroLibX, you just provide the shared library path in your compilation/linking command:

```sh
clang myApp.c /path/to/MacroLibX/libmlx.so -lSDL2
```

And you can enjoy your project

<p align="center">
    <img src="./res/screenshot_test.png" alt="drawing" width="400"/>
</p>

## ⚙️ Some compilation configurations

### 📦 Compile mode
By default the mlx is built in release mode but you can switch to debug by using `make DEBUG=true`.

### 🛠️ Set the toolchain
If you want to use `GCC` to build the mlx you can use `make TOOLCHAIN=gcc`

### ⚠️⚠️⚠️ 🖼️ Image optimisations ⚠️⚠️⚠️
If you run into glitches when writing or reading pixels from images you can turn off images optimisations by using `make IMAGES_OPTIMIZED=false`.

### 🖥️ Force the integrated GPU (not recommended)
You can force the mlx to use your integrated GPU by using `make FORCE_INTEGRATED_GPU=true`. Note that there are a lot of chances that your application crashes by using that.

### 💽 Dump the graphics memory
The mlx can dump it's graphics memory use to json files every two seconds by enabling this option `make GRAPHICS_MEMORY_DUMP=true`.

## License

This project and all its files, except the [`third_party`](./third_party) directory or unless otherwise mentionned, are licenced under the [MIT license](./LICENSE).
