# Copyright (c) 2014-2016, K. Kumar (me@kartikkumar.com)
# All rights reserved.

if (SGP4_INCLUDE_DIRS)
  # in cache already
  set(SGP4_FOUND TRUE)
else (SGP4_INCLUDE_DIRS)

  find_path(SGP4_INCLUDE_DIR
    NAMES
      SGP4-VER.TLE
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      sgp4 sgp4deorbit
  )

  find_library(SGP4_LIB_PATH
    NAMES
      sgp4
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      sgp4/libsgp4 sgp4/build sgp4deorbit/libsgp4 sgp4deorbit/build
    NO_DEFAULT_PATH
  )

  if(SGP4_INCLUDE_DIR AND SGP4_LIB_PATH)
    set(SGP4_INCLUDE_DIRS
      ${SGP4_INCLUDE_DIR}
    )
    get_filename_component(SGP4_LIBRARY_DIR ${SGP4_LIB_PATH} DIRECTORY)
    set(SGP4_LIBRARY "sgp4")
  endif(SGP4_INCLUDE_DIR AND SGP4_LIB_PATH)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(SGP4 DEFAULT_MSG SGP4_INCLUDE_DIRS)

  # show the SGP4_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(SGP4_INCLUDE_DIRS)

endif (SGP4_INCLUDE_DIRS)
