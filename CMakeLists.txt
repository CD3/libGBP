cmake_minimum_required (VERSION 2.6)
project(hgbp)


option( BUILD_SWIG_WRAPPERS    "Build interface wrapper libraries for other languages with SWIG" OFF )
option( GENERATE_SWIG_WRAPPERS "Generate new SWIG wrappers to build. The difference between this and BUILD_SWIG_WRAPPERS is that the latter
                                option will build wrapper libraries from
                                existing SWIG generated files. If you want
                                to generate new files to build with,
                                you should turn both of these options on" OFF )


find_package( Eigen3 REQUIRED )
if(EIGEN3_FOUND)
  include_directories( ${EIGEN3_INCLUDE_DIR} )
endif(EIGEN3_FOUND)
add_definitions(-Wno-deprecated-declarations)

find_package(Doxygen)

set( SOURCES )


#add_library(GBP ${SOURCES})
#target_link_libraries(GBP ${EXTERNAL_LIBS})

if(DOXYGEN_FOUND)
configure_file( ${PROJECT_SOURCE_DIR}/GBP.doxygen.in ${CMAKE_CURRENT_BINARY_DIR}/GBP.doxygen @ONLY)

add_custom_target(doc
                  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/GBP.doxygen
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating API documentation with Doxygen" VERBATIM
                  )
endif()

include_directories(src/)
add_subdirectory( swig )
add_subdirectory( testing )

