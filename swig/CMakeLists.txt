message( STATUS "Configuring SWIG interface build for libGBP" )

# allow the caller to disable certain wrappers
if(    NOT DEFINED PYTHON_WRAPPERS )
  set( PYTHON_WRAPPERS ON )
endif( NOT DEFINED PYTHON_WRAPPERS )

if( GENERATE_SWIG_WRAPPERS )
  if( PYTHON_WRAPPERS )
    message(STATUS "\tGenerage Python wrapper source is enabled" )
  endif( PYTHON_WRAPPERS )
endif( GENERATE_SWIG_WRAPPERS )

if( BUILD_SWIG_WRAPPERS )
  if( PYTHON_WRAPPERS )
    message(STATUS "\tBuild Python wrappers is enabled" )
  endif( PYTHON_WRAPPERS )
endif( BUILD_SWIG_WRAPPERS )


set( SWIG_PYTHON_MODULE_NAME        "py_libGBP" )
set( SWIG_PYTHON_OUTPUT_DIR         "${PROJECT_SOURCE_DIR}/swig/" )
set( SWIG_PYTHON_WRAPPER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/swig" )
set( SWIG_PYTHON_MODULE_FILE        "${SWIG_PYTHON_OUTPUT_DIR}/${SWIG_PYTHON_MODULE_NAME}.py" )
set( SWIG_PYTHON_WRAPPER_SOURCE     "${SWIG_PYTHON_WRAPPER_SOURCE_DIR}/${SWIG_PYTHON_MODULE_NAME}_wrap.cpp" )
set( SWIG_PYTHON_WRAPPER_HEADER     "${SWIG_PYTHON_WRAPPER_SOURCE_DIR}/py_${SWIG_PYTHON_MODULE_NAME}.h" )


if( BUILD_SWIG_WRAPPERS OR GENERATE_SWIG_WRAPPERS )
  if( PYTHON_WRAPPERS )
    find_package( PythonLibs 2.7 REQUIRED )
    include_directories(${PYTHON_INCLUDE_PATH})
  endif( PYTHON_WRAPPERS )
endif( BUILD_SWIG_WRAPPERS OR GENERATE_SWIG_WRAPPERS )

#  ____                           _        __        __                                   
# / ___| ___ _ __   ___ _ __ __ _| |_ ___  \ \      / / __ __ _ _ __  _ __   ___ _ __ ___ 
#| |  _ / _ \ '_ \ / _ \ '__/ _` | __/ _ \  \ \ /\ / / '__/ _` | '_ \| '_ \ / _ \ '__/ __|
#| |_| |  __/ | | |  __/ | | (_| | ||  __/   \ V  V /| | | (_| | |_) | |_) |  __/ |  \__ \
# \____|\___|_| |_|\___|_|  \__,_|\__\___|    \_/\_/ |_|  \__,_| .__/| .__/ \___|_|  |___/
#                                                              |_|   |_|                  

if( GENERATE_SWIG_WRAPPERS )
  find_package(SWIG REQUIRED)

    # we need the .i file to be in the binary directory so swig will generate files in the places we want them
    add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/libGBP.i
                        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libGBP.i
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${CMAKE_CURRENT_SOURCE_DIR}/libGBP.i"
                        "${CMAKE_CURRENT_BINARY_DIR}/libGBP.i"
                        COMMENT "copy swig interface file to binary dir" )



# ____        _   _                 
#|  _ \ _   _| |_| |__   ___  _ __  
#| |_) | | | | __| '_ \ / _ \| '_ \ 
#|  __/| |_| | |_| | | | (_) | | | |
#|_|    \__, |\__|_| |_|\___/|_| |_|
#       |___/                       

  if( PYTHON_WRAPPERS AND PYTHONLIBS_FOUND )

    get_property( dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach( dir ${dirs})
      list( APPEND libGBP_SWIG_PYTHON_FLAGS "-I${dir}" )
    endforeach( dir ${dirs})

    list( APPEND libGBP_SWIG_PYTHON_FLAGS -c++ -python                           )
    list( APPEND libGBP_SWIG_PYTHON_FLAGS -module ${SWIG_PYTHON_MODULE_NAME}     )
    list( APPEND libGBP_SWIG_PYTHON_FLAGS -outdir ${SWIG_PYTHON_OUTPUT_DIR}      )
    list( APPEND libGBP_SWIG_PYTHON_FLAGS -o      ${SWIG_PYTHON_WRAPPER_SOURCE}  )


    # it seems that swig does not generate a header file for python modules,
    # however, it also does not name the .cxx file what we want it to be, it
    # generates libGBP_wrap.cxx, but we would like py_libGBP_wrap.cxx
    add_custom_command( OUTPUT
                              # WARNING: specifiying multiple output files seems to cause this target to build every time for some reason
                              ${SWIG_PYTHON_MODULE_FILE}
                              ${SWIG_PYTHON_WRAPPER_SOURCE}
                              ${SWIG_PYTHON_WRAPPER_HEADER}
                        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/libGBP.i"
                        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/libGBP.i" "${CMAKE_CURRENT_BINARY_DIR}/libGBP.i"
                        # execute swig (in current binary directory)
                        COMMAND ${SWIG_EXECUTABLE} ${libGBP_SWIG_PYTHON_FLAGS} "libGBP.i"
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${SWIG_PYTHON_MODULE_FILE}"
                        "${CMAKE_CURRENT_BINARY_DIR}/${SWIG_PYTHON_MODULE_NAME}.py"
                        COMMENT "generate files for swig python wrapper" )

    if( NOT BUILD_SWIG_WRAPPERS )
      # if the build flag is not enabled, then we need to add a target that will
      # require the swig output files
      add_custom_target(py_libGBP_swig ALL
                        DEPENDS py_libGBP.py
                        )
    endif( NOT BUILD_SWIG_WRAPPERS )

  endif( PYTHON_WRAPPERS AND PYTHONLIBS_FOUND )


endif( GENERATE_SWIG_WRAPPERS )


# ____        _ _     _  __        __                                   
#| __ ) _   _(_) | __| | \ \      / / __ __ _ _ __  _ __   ___ _ __ ___ 
#|  _ \| | | | | |/ _` |  \ \ /\ / / '__/ _` | '_ \| '_ \ / _ \ '__/ __|
#| |_) | |_| | | | (_| |   \ V  V /| | | (_| | |_) | |_) |  __/ |  \__ \
#|____/ \__,_|_|_|\__,_|    \_/\_/ |_|  \__,_| .__/| .__/ \___|_|  |___/
#                                            |_|   |_|                  

if( BUILD_SWIG_WRAPPERS )

  # swig needs/wants a shared library to link against
  #add_library( GBP_shared SHARED ${libGBP_SOURCES})

  # ____        _   _                 
  #|  _ \ _   _| |_| |__   ___  _ __  
  #| |_) | | | | __| '_ \ / _ \| '_ \ 
  #|  __/| |_| | |_| | | | (_) | | | |
  #|_|    \__, |\__|_| |_|\___/|_| |_|
  #       |___/                       


  if(PYTHON_WRAPPERS AND PYTHONLIBS_FOUND)

    add_library( py_libGBP SHARED ${SWIG_PYTHON_WRAPPER_SOURCE}  )
    target_compile_features( py_libGBP PRIVATE cxx_auto_type)
    set_target_properties( py_libGBP PROPERTIES OUTPUT_NAME "_py_libGBP" )
    set_target_properties( py_libGBP PROPERTIES PREFIX      ""          )
    if(WIN32)
	    set_target_properties( py_libGBP PROPERTIES SUFFIX ".pyd" ) # on windows, the python module needs to have a .pyd extension
    endif(WIN32)
    target_link_libraries( py_libGBP ${PYTHON_LIBRARY} ${PYTHON_LIBRARIES} )


    add_custom_command( OUTPUT  "${CMAKE_BINARY_DIR}/python-unitTests"
                        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/unitTests/python"
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${CMAKE_CURRENT_SOURCE_DIR}/unitTests/python"
                        "${CMAKE_BINARY_DIR}/python-unitTests"
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${CMAKE_CURRENT_BINARY_DIR}/py_libGBP.py"
                        "${CMAKE_BINARY_DIR}/python-unitTests/"
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${CMAKE_CURRENT_BINARY_DIR}/_py_libGBP.so"
                        "${CMAKE_BINARY_DIR}/python-unitTests/"
                        COMMENT "copying python unit tests files" )
                      add_custom_target( copyPythonUnitTests ALL DEPENDS "${CMAKE_BINARY_DIR}/python-unitTests" py_libGBP)



  endif(PYTHON_WRAPPERS AND PYTHONLIBS_FOUND)

endif( BUILD_SWIG_WRAPPERS )
