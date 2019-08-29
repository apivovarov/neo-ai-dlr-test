# neo-ai-dlr-test

## Build

Make symlink to `neo-ai-dlr`. `neo-ai-dlr` should be pre-built and it should have `lib/libdlr.so`
```
ln -s <path_to_neo-ai-dlr>

make
```

`make` should generate `bin/run-resnet` and `bin/run-resnet2` exec files.

## Run
Set `LD_LIBRARY_PATH` to point to `neo-ai-dlr/lib` and try to run `bin/run-resnet`.

It will use pre-compiled resnetv18 model compiled for Intel skylake CPU.
```
export LD_LIBRARY_PATH=neo-ai-dlr/lib

bin/run-resnet

bin/run-resnet2
```
