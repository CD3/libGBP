cmake_minimum_required (VERSION 2.6)
project(hgbp)


set(CMAKE_CXX_FLAGS "-std=c++0x ")


include_directories("/usr/include/eigen3")


set( EXTERNAL_LIBS
    boost_system
    boost_filesystem )

set( SOURCES
            src/GaussianBeam.cpp )


add_library(GBP ${SOURCES})
target_link_libraries(GBP ${EXTERNAL_LIBS})


add_subdirectory( testing )
