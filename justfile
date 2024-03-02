list:
  just --list

install-deps-for-lib:
  conan export conan/recipes/unitconvert/conanfile.py
  conan install . --build missing -s build_type=Debug
  conan install . --build missing -s build_type=Release
  conan install . --build missing -s build_type=RelWithDebInfo

cmake-configure-lib: install-deps-for-lib
  cmake . -B build -DCMAKE_TOOLCHAIN_FILE=$( find ./build -name conan_toolchain.cmake )

build-lib:
  cmake --build build --config Debug

run-lib-tests: build-lib
  cd build && ./testBin/Debug/libGBP_CatchTests
  cd build && ./testing/Debug/libGBP2_lib_UnitTests

run-api-tests: build-lib
  cd build && ./testing/Debug/libGBP2_message_api_UnitTests

run-tests: build-lib
  cd build && ctest -C Debug --output-on-failure
