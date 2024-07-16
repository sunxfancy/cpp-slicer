C++ Slicer
=============

This a clang tool which can run on C++ source codebase.

## Dependencies

If you are developing on Ubuntu, you can install the following packages:

```
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
sudo apt install llvm-18-dev libclang-18-dev libzstd-dev
``` 

## Compiler

We need to use clang and lld to compile it. 