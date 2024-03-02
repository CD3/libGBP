cmake_minimum_required( VERSION 3.20 )

OPTION( BUILD_UNIT_TESTS "Build unit tests for the library" ON )


if(BUILD_UNIT_TESTS)

set( binDir ${CMAKE_BINARY_DIR}/testBin )

# Find all sources for the unit test executable.
file( GLOB_RECURSE unitTest_SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "./CatchTests/*.cpp" )
message(STATUS "Detected Catch-based Unit Test Sources:")
foreach(testSrc ${unitTest_SOURCES})
  message(STATUS "  ${testSrc}" )
endforeach()

add_executable(libGBP_CatchTests ${unitTest_SOURCES})
target_compile_definitions( libGBP_CatchTests PRIVATE -DTESTING )
target_link_libraries(libGBP_CatchTests GBP)
target_include_directories( libGBP_CatchTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" )

add_test(NAME libGBP_CatchTests
         COMMAND libGBP_CatchTests )




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
add_test(NAME libGBP2_lib_UnitTests COMMAND libGBP2_lib_UnitTests )



file( GLOB_RECURSE SOURCES
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
      "./libGBP2/CatchTests/message-api/*.cpp" )
message(STATUS "Detected libGBP2 message-api Catch-based Unit Test Sources:")
foreach(testSrc ${unitTest_SOURCES})
  message(STATUS "  ${testSrc}" )
endforeach()

add_executable(libGBP2_message_api_UnitTests ${SOURCES})
target_compile_definitions( libGBP2_message_api_UnitTests PRIVATE -DTESTING )
target_link_libraries(libGBP2_message_api_UnitTests  libGBP2::libGBP2-message-api Catch2::Catch2WithMain)
add_test(NAME libGBP2_api_UnitTests COMMAND libGBP2_lib_UnitTests )
endif()
