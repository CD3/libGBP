cmake_minimum_required (VERSION 2.6)
project(hgbp)


set(CMAKE_CXX_FLAGS "-std=c++0x ")


find_package( Eigen3 REQUIRED )
if(EIGEN3_FOUND)
  include_directories( ${EIGEN3_INCLUDE_DIR} )
endif(EIGEN3_FOUND)
add_definitions(-Wno-deprecated-declarations)


set( SOURCES )


#add_library(GBP ${SOURCES})
#target_link_libraries(GBP ${EXTERNAL_LIBS})


add_subdirectory( testing )
