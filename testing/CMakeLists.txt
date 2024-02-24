cmake_minimum_required( VERSION 3.20 )

OPTION( BUILD_UNIT_TESTS "Build unit tests for the library" ON )


if(BUILD_UNIT_TESTS)

set( binDir ${CMAKE_BINARY_DIR}/testBin )
set( buildDir ${CMAKE_BINARY_DIR} )

# Find all sources for the unit test executable.
file( GLOB_RECURSE unitTest_SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "./CatchTests/*.cpp" )
message(STATUS "Detected Catch-based Unit Test Sources:")
foreach(testSrc ${unitTest_SOURCES})
  message(STATUS "  ${testSrc}" )
endforeach()

set( testName ${PROJECT_NAME}_CatchTests )
add_executable(${testName} ${unitTest_SOURCES})
target_compile_definitions( ${testName} PRIVATE -DTESTING )
target_link_libraries(${testName} GBP)
target_include_directories( ${testName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${binDir})
set_target_properties(${testName} PROPERTIES CXX_STANDARD 17)

add_test(NAME ${testName} 
         WORKING_DIRECTORY ${buildDir}
         COMMAND ${binDir}/${testName} )

# Create a seprate executable for each "applet"
file( GLOB_RECURSE applet_SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "./Applets/*.cpp" )
foreach(testSrc ${applet_SOURCES})
  get_filename_component( testName ${testSrc} NAME_WE )
  message(STATUS "  Detected Applet ${testSrc}" )

  add_executable(${testName} ${testSrc})
  target_compile_definitions( ${testName} PRIVATE -DTESTING )
  target_link_libraries(${testName} GBP)
  target_include_directories( ${testName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" )
  set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${binDir})
  set_target_properties(${testName} PROPERTIES CXX_STANDARD 11)

  add_test(NAME ${testName} 
           WORKING_DIRECTORY ${buildDir}
           COMMAND ${binDir}/${testName} )

endforeach()



# libGBP2

find_package(Catch2 REQUIRED)
file( GLOB_RECURSE SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "./libGBP2/CatchTests/lib/*.cpp" )
message(STATUS "Detected libGBP2 library Catch-based Unit Test Sources:")
foreach(testSrc ${unitTest_SOURCES})
  message(STATUS "  ${testSrc}" )
endforeach()

add_executable(libGBP2_lib_UnitTests ${SOURCES})
target_compile_definitions( libGBP2_lib_UnitTests PRIVATE -DTESTING )
target_link_libraries(libGBP2_lib_UnitTests  libGBP2::libGBP2 Catch2::Catch2WithMain)


endif()
