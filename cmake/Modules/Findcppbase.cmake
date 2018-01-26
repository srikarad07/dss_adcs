# Copyright (c) <year> <author> (<email>)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

if(CPPBASE_INCLUDE_DIRS)
  # in cache already
  set(CPPBASE_FOUND TRUE)
else(CPPBASE_INCLUDE_DIRS)

  find_path(CPPBASE_INCLUDE_DIR
    NAMES
      cppbase.hpp
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      cppbase include/cppbase
  )

  if(CPPBASE_INCLUDE_DIR)
    set(CPPBASE_INCLUDE_DIRS
      ${CPPBASE_INCLUDE_DIR}/..
    )
  endif(CPPBASE_INCLUDE_DIR)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(cppbase DEFAULT_MSG CPPBASE_INCLUDE_DIRS)

  # show the CPPBASE_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(CPPBASE_INCLUDE_DIRS)

endif(CPPBASE_INCLUDE_DIRS)