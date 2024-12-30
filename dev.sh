cmake -B build -G Ninja && cmake --build build
./build/src/server/server &
LAST_PID=$!

#!/usr/bin/env bash
fswatch src -o -e ".*" -i "CMakeLists.txt$" src | xargs -n1 -I{} sh -c 'echo {} && cmake -B build -G Ninja' &

fswatch -o -e ".*" -i "\\.[ch]pp$" -o src | while read -r; do
  # Kill the previous command if it's still running
  if [ -n "$LAST_PID" ] && kill -0 "$LAST_PID" 2>/dev/null; then
    kill "$LAST_PID"
    wait "$LAST_PID" 2>/dev/null
  fi

  cmake --build build
  ./build/src/server/server &
  LAST_PID=$!
done
