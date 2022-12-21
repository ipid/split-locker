<h1 align="center">Split-locker</h1>
<p align="center"><i>A simple demonstration that shows how split-lock instructions can impact CPU performance.</i></p>

<br>

## Usage

Run the demonstration with 8 threads:

```Bash
./split-locker -t 8
```

<br>

## Build on Windows

You should use MinGW-w64 for compilation.

1. Install requirements:

```Bash
pacman -Sy mingw-w64-x86_64-cmake mingw-w64-clang-x86_64-toolchain
```

2. Build the project:

```Bash
# Create a standalone directory for building
mkdir build
cd build

# Build the project
cmake -G 'MinGW Makefiles' ..
mingw32-make
```

Then you should see `split-locker.exe` in the `build` directory.

<br>

## Build on Linux

1. Install CMake and build essentials:

```Bash
sudo apt install cmake build-essential
```

2. Build the project:

```Bash
# Create a standalone directory for building
mkdir build
cd build

# Build the project
cmake ..
make
```

<br>
