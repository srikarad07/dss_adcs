# Copyright (c) 2014-2015 Kartik Kumar, Dinamica Srl
# Copyright (c) 2014-2015 Marko Jankovic, DFKI GmbH
# Copyright (c) 2014-2015 Natalia Ortiz, University of Southampton
# Copyright (c) 2014-2015 Juan Romero, University of Strathclyde
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

if (SPOT_INCLUDE_DIRS)
  # in cache already
  set(SPOT_FOUND TRUE)
else (SPOT_INCLUDE_DIRS)

  find_path(SPOT_INCLUDE_DIR
    NAMES
      spot.hpp
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      spot/include/Spot include/Spot Spot
  )

  find_library(SPOT_LIB_PATH
    NAMES
      spot
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      spot spot/build spot/build/lib
    NO_DEFAULT_PATH
  )

  if(SPOT_INCLUDE_DIR AND SPOT_LIB_PATH)
    set(SPOT_INCLUDE_DIRS
      ${SPOT_INCLUDE_DIR}/..
    )
    get_filename_component(SPOT_LIBRARY_DIR ${SPOT_LIB_PATH} DIRECTORY)
    set(SPOT_LIBRARY "spot")
  endif(SPOT_INCLUDE_DIR AND SPOT_LIB_PATH)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Spot DEFAULT_MSG SPOT_INCLUDE_DIRS)

  # show the SPOT_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(SPOT_INCLUDE_DIRS)

endif (SPOT_INCLUDE_DIRS)
