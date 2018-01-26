# Copyright (c) 2014-2016, K. Kumar (me@kartikkumar.com)
# Distributed under the MIT License.
# See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT

if(ATOM_INCLUDE_DIRS)
  # in cache already
  set(ATOM_FOUND TRUE)
else(ATOM_INCLUDE_DIRS)

  find_path(ATOM_INCLUDE_DIR
    NAMES
      atom.hpp
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      Atom atom/include/Atom Atom/src/atom/include/Atom
  )

  if(ATOM_INCLUDE_DIR)
    set(ATOM_INCLUDE_DIRS
        ${ATOM_INCLUDE_DIR}/..
    )
  endif(ATOM_INCLUDE_DIR)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Atom DEFAULT_MSG ATOM_INCLUDE_DIRS)

  # show the ATOM_INCLUDE_DIRS variables only in the advanced view
  mark_as_advanced(ATOM_INCLUDE_DIRS)

endif(ATOM_INCLUDE_DIRS)