```sh
iwyu_tool.py -p build examples/soa_* -- $(for v in include/SOAExtensions/* ; do echo "--check_also=../$v" ; done) --mapping_file=../cppreference.imp
clang-tidy -p=build examples/soa*
```

