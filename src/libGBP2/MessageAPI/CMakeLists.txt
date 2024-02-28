find_package(protobuf REQUIRED)
find_package(UnitConvert REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Messages.proto)

add_library(libGBP2-protobuf)
add_library(libGBP2::libGBP2-protobuf ALIAS libGBP2-protobuf)
target_sources(libGBP2-protobuf PRIVATE ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(
  libGBP2-protobuf PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries( libGBP2-protobuf PUBLIC protobuf::protobuf)

protobuf_generate_python(PROTO_PY Messages.proto)
add_custom_target(python_protobuf ALL DEPENDS ${PROTO_PY})

# copy our protobuf file and python bindings into the install directory
# so users can have them
install(FILES Messages.proto "${PROTO_PY}" DESTINATION protobuf)

add_library( libGBP2-message-api)
add_library(libGBP2::libGBP2-message-api ALIAS libGBP2-message-api)
target_sources(libGBP2-message-api PRIVATE Propagator.cpp Messages.cpp)

target_include_directories(
  libGBP2-message-api PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
                     target_link_libraries( libGBP2-message-api PUBLIC libGBP2 libGBP2-protobuf UnitConvert::UnitConvert)


