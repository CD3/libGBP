cmake_minimum_required( VERSION 3.20 )

option(PYTHON_BINDINGS "Build Python bindings." OFF)

option( BUILD_TESTS "Build unit tests" ON )
if( ${BUILD_TESTS} )
enable_testing()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libGBP)
include(macro-git_version)
GIT_VERSION(libGBP)
message( STATUS "libGBP version: ${libGBP_VERSION}")

# need to run configure_file twice
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/version.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/include/libGBP/version.h")
configure_file( "${CMAKE_CURRENT_BINARY_DIR}/include/libGBP/version.h"
                "${CMAKE_CURRENT_BINARY_DIR}/include/libGBP/version.h")


set( cxx_needed_features
     cxx_auto_type
   )

find_package( Boost REQUIRED )
find_package( BoostUnitDefinitions REQUIRED )
find_package( Eigen3 3.0 REQUIRED)

string( REGEX REPLACE "^lib" "" LIB_NAME ${PROJECT_NAME} )
add_library( ${LIB_NAME} INTERFACE )
add_library( ${PROJECT_NAME}::${LIB_NAME} ALIAS ${LIB_NAME} )
target_include_directories( ${LIB_NAME} INTERFACE
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${${PROJECT_NAME}_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/>
)
target_sources(
  ${LIB_NAME}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Units.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/GaussianBeam.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/GBPCalc.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/OpticalSystem.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/MediaStack.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Constants.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/utils/ptree.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Media/BaseMedia.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Media/LinearAbsorber.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Media/MediaInterface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/BeamTransformation_Interface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/BeamTransformation_Base.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/FlatInterface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/SphericalInterface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/Interface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/ThinLens.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/BeamTransformations/Filter.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Builders/Builder.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Builders/MediaBuilder.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Builders/OpticalSystemBuilder.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Builders/OpticalElementBuilder.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Builders/BeamBuilder.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libGBP/Builders/MediaStackBuilder.hpp>
)

target_link_libraries(${LIB_NAME} INTERFACE Boost::boost Eigen3::Eigen BoostUnitDefinitions::BoostUnitDefinitions)

if( BUILD_TESTS)
add_subdirectory( testing )
configure_file( GBP.doxygen.in ${CMAKE_CURRENT_BINARY_DIR}/GBP.doxygen )
endif()

install( TARGETS ${LIB_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include/libGBP
)
install(DIRECTORY src/
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION cmake
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
"include(CMakeFindDependencyMacro)
find_dependency(Boost)
find_dependency( Eigen3 )
find_dependency( BoostUnitDefinitions )
add_library( Eigen3::eigen3 INTERFACE IMPORTED )
set_property( TARGET Eigen3::eigen3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${EIGEN3_INCLUDE_DIR})
include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)
"
  )
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
  )
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION cmake
  )




INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ library doing calculations with Gaussian beams.")
SET(CPACK_PACKAGE_VENDOR "C.D. Clark III")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
if( ${PROJECT_NAME}_VERSION_MAJOR)
  SET(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
endif()
if( ${PROJECT_NAME}_VERSION_MINOR)
  SET(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
endif()
if( ${PROJECT_NAME}_VERSION_PATCH)
  SET(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH})
endif()
INCLUDE(CPack)




if(PYTHON_BINDINGS)
  find_package(Python3 REQUIRED COMPONENTS Development)
  find_package(SWIG REQUIRED)
  include(${SWIG_USE_FILE})
  set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/swig/libGBP.i" PROPERTY CPLUSPLUS ON)
  swig_add_library( py${LIB_NAME} TYPE SHARED LANGUAGE python SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/swig/libGBP.i")
  swig_link_libraries(py${LIB_NAME} PRIVATE ${LIB_NAME} Python3::Module)
endif()
