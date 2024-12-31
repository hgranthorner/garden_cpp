#!/usr/bin/env bash

# Set build type from first argument, default to Debug
BUILD_TYPE="${1:-Debug}"

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE="$BUILD_TYPE" && cmake --build build
./build/src/server/server &
LAST_PID=$!

fswatch src -o -e ".*" -i "CMakeLists.txt$" src | xargs -n1 -I{} sh -c "echo {} && cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE" &

fswatch -o -e ".*" -i "\\.[ch]pp$" -o src/server | while read -r; do
  # Kill the previous command if it's still running
  if [ -n "$LAST_PID" ] && kill -0 "$LAST_PID" 2>/dev/null; then
    kill "$LAST_PID"
    wait "$LAST_PID" 2>/dev/null
  fi

  cmake --build build --target server compile_commands
  ./build/src/server/server &
  LAST_PID=$!
done &

fswatch -o -e ".*" -i "\\.[ch]pp$" -o src/client | while read -r; do
  cmake --build build --target client compile_commands
done

fswatch -o -e ".*" -i "\\.[ch]pp$" -o src/shared | while read -r; do
  cmake --build build 
done
