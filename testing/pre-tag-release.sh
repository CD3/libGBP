#! /bin/bash

set -e
root=$(git rev-parse --show-toplevel)

cd $root

bindir="$PWD/build-and-test"
function cleanup()
{
  rm -r $bindir
}
trap cleanup EXIT
function copy_state()
{
  cp -r $bindir $bindir.error
}
trap copy_state ERR

mkdir $bindir
cd $bindir
conan install ..
source activate.sh
cmake .. -DCMAKE_INSTALL_PREFIX=$bindir/install
source deactivate.sh
cmake --build .
cmake --build . --target test

# test install
cmake --build . --target install

mkdir app
cd app

cat << EOF > main.cpp
#include <iostream>
#include <libGBP_version.h>
#include <GaussianBeam.hpp>

int main()
{
  std::cout << "VERSION: " << libGBP_VERSION << std::endl;
  std::cout << "FULL VERSION: " << libGBP_VERSION_FULL << std::endl;

  GaussianBeam beam;

  return 0;
}
EOF

cat << EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.1)
add_executable( main main.cpp )
find_package( libGBP REQUIRED )
target_link_libraries(main libGBP::GBP )
set_target_properties(main PROPERTIES CXX_STANDARD 11)
EOF

mkdir build1
cd build1
source activate.sh
conan install ..
source deactivate.sh
cmake .. -DlibGBP_DIR=${bindir}/install/cmake/
cmake --build .
./main

cd ..

cat << EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.1)
add_executable( main main.cpp )
find_package( libGBP REQUIRED PATHS ${bindir}/install )
target_link_libraries(main libGBP::GBP )
set_target_properties(main PROPERTIES CXX_STANDARD 11)
EOF

mkdir build2
cd build2
conan install ..
source activate.sh
cmake ..
source deactivate.sh
cmake --build .
./main

echo "PASSED"

exit 0
