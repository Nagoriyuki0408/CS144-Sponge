rm -rf build
cmake -S . -B build
cmake --build build
cmake --build build --target check_lab