# Copyright (c) 2014-2016, K. Kumar (me@kartikkumar.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

if(PYKEP_INCLUDE_DIRS)
  # in cache already
  set(PYKEP_FOUND TRUE)
else(PYKEP_INCLUDE_DIRS)

  find_path(PYKEP_INCLUDE_DIR
    NAMES
      keplerian_toolbox.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      keplerian_toolbox keplerian_toolbox/src
      pykep pykep/src
      keptool keptool/src
  )

  find_library(PYKEP_LIB_PATH
    NAMES
      keplerian_toolbox_static
      keplerian_toolbox
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      keplerian_toolbox/src keplerian_toolbox/build keplerian_toolbox/build/src
      pykep/src/ pykep/build pykep/build/src
      keptool/src keptool/build keptool/build/src
    NO_DEFAULT_PATH
  )

  if(PYKEP_INCLUDE_DIR AND PYKEP_LIB_PATH)
    set(PYKEP_INCLUDE_DIRS
      ${PYKEP_INCLUDE_DIR}
    )
    get_filename_component(PYKEP_LIBRARY_DIR ${PYKEP_LIB_PATH} DIRECTORY)
    set(PYKEP_LIBRARY "keplerian_toolbox_static")
  endif(PYKEP_INCLUDE_DIR AND PYKEP_LIB_PATH)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(PyKEP DEFAULT_MSG PYKEP_INCLUDE_DIRS)

  # show the PYKEP_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(PYKEP_INCLUDE_DIRS)

endif(PYKEP_INCLUDE_DIRS)