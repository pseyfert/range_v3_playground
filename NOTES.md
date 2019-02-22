```sh
iwyu_tool.py -p build examples/soa_* -- --check_also=../include/SOAContainerSet.h --check_also=../include/PrSelection.h --check_also=../include/IncompatibleZipException.h --check_also=../include/ZipTraits.h --check_also=../include/ZipUtils.h
clang-tidy -p=build examples/soa
```

