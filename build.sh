#!/bin/bash

# Exit on error
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Check if EMSDK environment is set
if [ -z "$EMSDK" ]; then
    echo -e "${RED}Error: EMSDK environment variable not set${NC}"
    echo "Please source your emsdk_env.sh first:"
    echo "source /path/to/emsdk/emsdk_env.sh"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
    echo "Created build directory"
fi

# Navigate to build directory
cd build

# Configure with CMake
echo -e "${GREEN}Configuring with CMake...${NC}"
$EMSDK/upstream/emscripten/emcmake cmake ..

# Build the project
echo -e "${GREEN}Building project...${NC}"
emmake make -j$(nproc)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build completed successfully!${NC}"
    echo "You can find the output files in the build directory"
    echo "To serve the files locally, you can run:"
    echo "python3 -m http.server --directory build"
else
    echo -e "${RED}Build failed${NC}"
    exit 1
fi

