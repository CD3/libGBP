list:
  just --list

install-deps-for-lib:
  conan export conan/recipes/unitconvert/conanfile.py
  conan install . --build missing -s build_type=Debug

cmake-configure-lib: install-deps-for-lib
  cmake . -B build -DCMAKE_TOOLCHAIN_FILE=./build/Debug/generators/conan_toolchain.cmake

build-lib: cmake-configure-lib
  cmake --build build --config Debug

run-lib-tests: build-lib
  cd build && ./testBin/Debug/libGBP_CatchTests
  cd build && ./testing/Debug/libGBP2_lib_UnitTests

run-api-tests: build-lib
  cd build && ./testing/Debug/libGBP2_message_api_UnitTests
