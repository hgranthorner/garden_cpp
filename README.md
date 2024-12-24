`source ../emsdk/emsdk_env.sh`
emsdk activate 3.1.74
cmake -B build && cmake --build build
python3 -m http.server -d build
