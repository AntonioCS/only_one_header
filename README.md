
# only_one_header
Small application to put all header files into one (Still WIP)

```
Usage:
onlyoneheader [-h] [--output=<file>] [--remove_comments] [--no_separators] [--no_timestamp] [--silent] <path>

    -h --help                   show this
    --output=<file>             specify output file [default: ./allinone.h]
    --remove_comments           remove one line comments from the header files
    --no_separators             don't add // <filename> START|END separators
    --no_timestamp              don't add timestamp
    --silent                    no info text
```

**<path>**  Is the directory path of where the header files are. Header files with extension .h and .hpp will be consumed.

Use cmake to compile :

- mkdir build/
- cd build/
- cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="x64-windows"

To simplify, if you are on windows, you can use this [cmake.bat](https://gist.github.com/AntonioCS/81e96d20bc8538db8a2e469f01294f6f).

This program depends on [docopt](https://github.com/docopt/docopt.cpp) (for cli options) and [catch2](https://github.com/catchorg/Catch2) (for future tests).
