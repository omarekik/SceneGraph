#!/bin/bash -x

#
# first parameter of this script is the build type which is either Debug or Release
#
if [ -z "$1" ]
  then
    echo "No argument supplied"
    exit 1
fi
build_type_argument=$1

# Clean build folder
rm -rf build

# Build & Run unit tests & create conan package
conan profile detect --force

# Install conan package
conan install . --output-folder=build --build=missing --settings=build_type=$build_type_argument

# Generate build system
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$build_type_argument

# Build
cmake --build . --config $build_type_argument

# Run test scripts
ctest .

# Print help message
./main/Start -h 

# Create a start
./main/Start --create --width 10

# Return back from build folder to project folder
cd ..
