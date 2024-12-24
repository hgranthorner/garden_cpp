# clang++ --target=wasm32 \
#   -stdlib=libc++ \
#   -nostdlib++ \
#   -I/path/to/libc++/include \
#   -I/path/to/libc++abi/include \
#   -L/path/to/libc++/lib \
#   -L/path/to/libc++abi/lib \
#   -lc++ \
#   -lc++abi \
#   -o output.wasm \
#   your_code.cpp
#
prefix="$(brew --prefix llvm)"

clang++ --target=wasm32 \
  -D_LIBCPP_HAS_NO_THREADS \
  -D_LIBCPP_HAS_NO_ALIGNED_ALLOCATION \
  -stdlib=libc++ \
  -nostdlib++ \
  -nostdinc++ \
  "-I$prefix/include/c++/v1" \
  "-I$prefix/include" \
  "-L$prefix/lib" \
  -lc++ \
  -lc++abi \
  -o output.wasm \
  main.cpp
