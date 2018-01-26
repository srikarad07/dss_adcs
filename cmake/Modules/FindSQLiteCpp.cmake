# @file    CMakeLists.txt
# @ingroup SQLiteCpp
# @brief   SQLiteCpp CMake module.
#
# Copyright (c) 2010-2014 Kartik Kumar (me@kartikkumar.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)

macro(_sqlitecpp_check_version)
  file(READ "${SQLITECPP_INCLUDE_DIR}/SQLiteCpp.h" _sqlitecpp_header)

  string(REGEX MATCH "define[ \t]+SQLITECPP_VERSION_NUMBER[ \t]+([0-9]+)"
         _sqlitecpp_version_match "${_sqlitecpp_header}")
  set(SQLITECPP_VERSION "${CMAKE_MATCH_1}")
  if(${SQLITECPP_VERSION} VERSION_LESS ${SQLiteCpp_FIND_VERSION})
    set(SQLITECPP_VERSION_OK FALSE)
  else(${SQLITECPP_VERSION} VERSION_LESS ${SQLiteCpp_FIND_VERSION})
    set(SQLITECPP_VERSION_OK TRUE)
  endif(${SQLITECPP_VERSION} VERSION_LESS ${SQLiteCpp_FIND_VERSION})

  if(NOT SQLITECPP_VERSION_OK)
    message(STATUS "SQLiteCpp version ${SQLITECPP_VERSION} found in ${SQLITECPP_INCLUDE_DIR}, "
                   "but at least version ${SQLiteCpp_FIND_VERSION} is required!")
  endif(NOT SQLITECPP_VERSION_OK)

  set(SQLITECPP_LIBRARY "SQLiteCpp")
  link_directories(${SQLITECPP_LIBRARY_DIR})
endmacro(_sqlitecpp_check_version)

if(SQLITECPP_INCLUDE_DIRS)

  # Check if SQLiteCpp is already present in cache.
  _sqlitecpp_check_version()
  set(SQLITECPP_FOUND
    ${SQLITECPP_VERSION_OK})

else(SQLITECPP_INCLUDE_DIRS)

  find_path(SQLITECPP_INCLUDE_DIR
    NAMES
      SQLiteCpp.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      SQLiteCpp SQLiteCpp/include/SQLiteCpp SQLiteCpp/src/sqlitecpp/include/SQLiteCpp
    )

  find_library(SQLITECPP_LIB_PATH
    NAMES
      SQLiteCpp
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      /usr/local
      ${PROJECT_PATH}/..
      ${EXTERNAL_PATH}
    PATH_SUFFIXES
      SQLiteCpp SQLiteCpp/build SQLiteCpp/src/sqlitecpp/build
    NO_DEFAULT_PATH
  )

  if(SQLITECPP_INCLUDE_DIR AND SQLITECPP_LIB_PATH)
    set(SQLITECPP_INCLUDE_DIRS
      ${SQLITECPP_INCLUDE_DIR}/..)
    _sqlitecpp_check_version()
    get_filename_component(SQLITECPP_LIBRARY_DIR ${SQLITECPP_LIB_PATH} DIRECTORY)
    set(SQLITECPP_LIBRARY "SQLiteCpp")
  endif(SQLITECPP_INCLUDE_DIR AND SQLITECPP_LIB_PATH)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    SQLITECPP DEFAULT_MSG SQLITECPP_INCLUDE_DIR SQLITECPP_VERSION_OK)

  mark_as_advanced(SQLITECPP_INCLUDE_DIRS SQLITECPP_LIBRARY)

endif(SQLITECPP_INCLUDE_DIRS)
