```sh
iwyu_tool.py -p build examples/soa_* -- $(for v in include/SOAExtensions/* ; do echo "-Xiwyu --check_also=../$v" ; done) -Xiwyu --mapping_file=../cppreference.imp
clang-tidy -p=build examples/soa*
```

