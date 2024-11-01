conan profile detect || true
conan install network --output-folder=network/build --build=missing
cd network/build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
#cmake --build .
make -j8
