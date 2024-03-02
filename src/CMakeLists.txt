

add_library( libGBP2 INTERFACE )
add_library( libGBP2::libGBP2 ALIAS libGBP2 )
target_link_libraries(libGBP2 INTERFACE Boost::boost Eigen3::Eigen BoostUnitDefinitions::BoostUnitDefinitions)

target_include_directories( libGBP2 INTERFACE
  $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${${PROJECT_NAME}_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/>
)
target_sources(
  libGBP2
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/Units.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalSystem.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/MonochromaticSource.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/CircularLaserBeam.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/CircularGaussianLaserBeam.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/Conventions.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalElements/OpticalElement.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalElements/FlatRefractiveSurface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalElements/SphericalRefractiveSurface.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalElements/ThickLens.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalElements/ThinLens.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalElements/FreeSpace.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/OpticalSystem.hpp>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libGBP2/Propagation.hpp>
  )


add_subdirectory(libGBP2/MessageAPI)
