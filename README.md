<div align="center">
    <img src="./res/logo.png" alt="drawing" width="200"/>
    <div align="center">
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_clang.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_clang.yml/badge.svg"></a>
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_gcc.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/linux_gcc.yml/badge.svg"></a>
        <a href="https://github.com/420verfl0w/MacroLibX/actions/workflows/macos_x86.yml"><img src="https://github.com/420verfl0w/MacroLibX/actions/workflows/macos_x86.yml/badge.svg"></a>
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

### Clone and Build

Finally, you can clone the Git repository. When inside it, run the GNU `make` command to compile MacroLibX. 
```bash
git clone https://github.com/420verfl0w/MacroLibX.git
cd MacroLibX
make
```

If you want to build using GCC you can add `TOOLCHAIN=gcc` to your `make` command
```bash
make TOOLCHAIN=gcc
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

### ⚠️ Troubleshooting

#### If you run into glitches when writing or reading pixels from images

You need to add `IMAGES_OPTIMIZED=false` to your `make` command:

```sh
# In your cloned environment, inside the MacroLibX folder
make IMAGES_OPTIMIZED=false
```

## License

This project and all its files, except the [`third_party`](./third_party) directory or unless otherwise mentionned, are licenced under the [MIT license](./LICENSE).
