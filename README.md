# Running
1. Clone the repo
2. Set up your emscripten sdk environment
3. Source and activate the emsdk environment
4. `cmake -B build -G Ninja && cmake --build build`
5. `./build/src/server/server`

For interactive development, just run `./dev.sh` in a separate terminal. It will handle rebuilding and recompiling the application as necessary.

Note that you will need a scraped set of data from `trefle.io` available at `./db.sqlite3`.
