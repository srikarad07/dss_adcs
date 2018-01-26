# Copyright (c) 2014-2016, K. Kumar (me@kartikkumar.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

if(SML_INCLUDE_DIRS)
  # in cache already
  set(SML_FOUND TRUE)
else(SML_INCLUDE_DIRS)

  find_path(SML_INCLUDE_DIR
    NAMES
      sml.hpp
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      sml sml/include/sml sml/src/sml/include/sml
  )

  if(SML_INCLUDE_DIR)
    set(SML_INCLUDE_DIRS
        ${SML_INCLUDE_DIR}/..
    )
  endif(SML_INCLUDE_DIR)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(sml DEFAULT_MSG SML_INCLUDE_DIRS)

  # show the SML_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(SML_INCLUDE_DIRS)

endif(SML_INCLUDE_DIRS)
