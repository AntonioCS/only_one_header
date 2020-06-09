# only_one_header
Small application to put all header files into one

```
Usage: onlyoneheader [-h] [--output=<file>] [--remove_comments] [--no_separators] [--no_timestamp] [--silent] <path>

    -h --help                   show this
    --output=<file>             specify output file [default: ./allinone.h]
    --remove_comments           remove one line comments from the header files
    --no_separators             don't add // <filename> START|END separators
    --no_timestamp              don't add timestamp
    --silent                    no info text
```

<path> Is the directory path of where the header files are. Header files with extension .h and .hpp will be consumed