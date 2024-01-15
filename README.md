## Build

```shell
# Create Conan profile
$ conan profile detect --force
# Generate required CMake files
$ conan install . --output-folder=build --build=missing
# Move to build directory
$ cd build
# Generate build files for Visual Studio 2019
$ cmake .. -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
# Build
$ cmake --build . --config Release
```

> **PowerShell Note:** Wrap "conan_toolchain.cmake" with quotes.
> **Unix Note:** Add `-DCMAKE_BUILD_TYPE=Debug|Release` flag if you're build for a POSIX system.

### Generate Compile Commands File

From `build` directory, run this command:

```sh
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
```

Then, move the `build/compile_commands.json` file to your project root.
