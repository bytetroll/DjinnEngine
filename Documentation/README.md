# Djinn Engine

*** Note ***
This documentation is a living document and a work in progress.
This documentation may radially change throughout Djinn's development.

## 1 Build Instructions

Djinn Engine is primarily developed on the CLion IDE, some of the following build steps are 
only applicable to CLion.

### 1.1 Toolchain configuration

Please see the respective build instructions for your platform.

#### 1.1.1 Windows

Prerequisites:

- Visual Studio Toolchain, 2017 and above. Edition does not matter.

  https://visualstudio.microsoft.com/downloads/

- Ninja
  
  https://github.com/ninja-build/ninja/releases

- Clang/LLVM 10.0.0, see under "Pre-Built Binaries"

  https://releases.llvm.org/download.html

After installing all prerequisites, navigate to the toolchain setup, `File -> Settings -> Build, Execution, Deployment -> Toolchains`.
Create a new `Visual Studio` toolchain (click +, top left).

Ensure the `Architecture` is `amd64`, and set the following paths (replace `%X%` with the appropriate installation path):
- `Make` : `%NINJA_PATH%\ninja.exe`
- `C Compiler` : `%LLVM_PATH%\bin\clang-cl.exe`
- `C++ Compiler` : `%LLVM_PATH%\bin\clang-cl.exe`

**Ensure the C/C++ compiler is assigned to the `-cl.exe` postfixed compiler.**

Finally, ensure the `Debugger` is `Bundled LLDB` to enable debugging.

#### 1.1.2 Linux

While Djinn will run (and develop) on any Linux flavor, Djinn has a preferred.  Djinn is being actively
developed on (and against) Arch Linux (specifically Manjaro).  Manjaro was chosen due to bleeding
edge availability, as well as ease of development environment setup (especially pertaining to graphics
driver(s)).

Note: Fedora was the active development flavor until version ${VERSION} with Ubuntu predating Fedora.
As a result, this documentation will provide setup instructions for Debian (Ubuntu), RHL (Fedora), and 
Arch (Manjaro) with Arch being the preferred flavor for development.

Note: There is currently a bug in DHG that causes the initial build to fail due to DHG not running before
the build.  When building on Linux, build first to allow DHG to generate the files followed by a second
build.

Prerequisites:

- Vulkan

Special Note: Along with the appropriate vulkan loader for your architecture, you will also
need the coresponding graphics driver.  On Arch these are

    Intel: vulkan-intel

    NVIDIA: nvidia

    AMD: vulkan-radeon (for the radeon driver or AMDGPU)
  
  Arch: 'sudo pacman -S vulkan-icd-loader'

- Ninja 

  Arch: `sudo pacman -S ninja`
  
  RHL: `sudo yum install ninja-build`
  
  Debian: `sudo apt-get install ninja-build`

- Clang 10.0.0

  Arch: `sudo pacman -S clang`
  
  RHS `sudo yum install clang`
  
  Debian: `sudo apt-get install clang-10`

- lld 10.0.0

  Arch: `sudo pacman -S lld`
  
  RHL: `sudo yum install lld`
  
  Debian: `sudo apt-get install lld-10`

- Libc++ 10.0.0

  Arch: `sudo pacman install libc++ libc++-abi`
  
  RHL: `sudo yum install libc++-10-dev libc++abi-10-dev`
  
  Debian: `sudo apt-get install libc++-10-dev libc++abi-10-dev`

System library prerequisites:

- X11

  Arch: `sudo pacman -S libx11-dev`
  
  RHL: `sudo yum install libx11-dev`
  
  Debian: `sudo apt install libx11-dev`

- NCurses

  Arch: `sudo pacman -S ncurses`

  RHL: `sudo yum install ncurses`

  Debian: `sudo apt install libncurses5`

***

Ncurses bleeding edge is v6.0 which is backwards compatible with 5.  If receiving link errors, first check that
you're running version 5 and symlink if so.

`ls /lib/ | grep libtinfo`

`sudo link /lib/libtinfo.so.6 /lib/libtinfo.so.5`

***

After installing all prerequisites, navigate to the toolchain setup, `File -> Settings -> Build, Execution, Deployment -> Toolchains`.
Either create a new `System` toolchain (click +, top left) or modify the existing.

Set the following paths:
- `Make` : `/bin/ninja.exe`
- `C Compiler` : `/bin/clang-10`
- `C++ Compiler` : `/bin/clang++-10`

### 1.2 CMake configuration

After configuring the toolchain, navigate to the CMake setup, `File -> Settings -> Build, Execution, Deployment -> CMake`.
Either create a new profile (click +, left) or modify an existing. Djinn supports one of four build types:
- `Debug64`
  
  No optimizations with debugging support.

- `Release64`
  
  Production build, optimized for speed.

- `Profile64`

  Most optimizations with debugging support, useful for debugging timing related issues.

- `Verify64`

  Not implemented yet, but will be a validation build. Enables clang address, thread, ... sanitizers.

Assign the build type to one of those, and set the `CMake Options` to `-G Ninja -DCMAKE_LINKER=lld`.
You can optionally assign the core count in `Build options` with `-j #`.