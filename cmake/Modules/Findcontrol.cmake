# Copyright (c) 2014-2016, K. Kumar (me@kartikkumar.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

if(CONTROL_INCLUDE_DIRS)
  # in cache already
  set(CONTROL_FOUND TRUE)
else(CONTROL_INCLUDE_DIRS)

  find_path(CONTROL_INCLUDE_DIR
    NAMES
      control.hpp
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      control control/include/control control/src/control/include/control
  )

  if(CONTROL_INCLUDE_DIR)
    set(CONTROL_INCLUDE_DIRS
        ${CONTROL_INCLUDE_DIR}/..
    )
  endif(CONTROL_INCLUDE_DIR)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(control DEFAULT_MSG CONTROL_INCLUDE_DIRS)

  # show the CONTROL_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(CONTROL_INCLUDE_DIRS)

endif(CONTROL_INCLUDE_DIRS)
