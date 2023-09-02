#!/bin/bash

# Step 1: Create a build directory and navigate to it
mkdir -p build
cd build

# Step 2: Run CMake to generate build files
cmake ..

# Step 3: Build the project
make

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Step 4: Run the raytracer executable
    /home/levent/Projects/ray-tracer/build/raytracer

    # Step 5: Display the generated image using feh (if available)
    if command -v feh &>/dev/null; then
        feh out.ppm
    else
        echo "feh is not installed. You can view out.ppm using an image viewer."
    fi
else
    echo "Build failed. Check for compilation errors."
fi

