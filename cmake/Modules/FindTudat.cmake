 #    Copyright (c) 2010-2016, Delft University of Technology
 #    All rights reserved.
 #    See LICENSE for license details
 #    (http://tudat.tudelft.nl/projects/tudat/repository/tudat/entry/trunk/LICENSE).
 #
 #    Changelog
 #      YYMMDD    Author            Comment
 #      12xxxx    B. Tong Minh      File created based on FindEigen3.cmake.
 #
 #    References
 #      FindEigen3.cmake.
 #
 #    Notes
 #      This script tries to find the Tudat library. This module supports requiring a minimum
 #      version, e.g. you can do find_package(Tudat 3.1.2) to require version 3.1.2 or newer of
 #      Tudat.
 #
 #      Once done, this will define:
 #
 #          TUDAT_FOUND - system has Tudat lib with correct version;
 #          TUDAT_INCLUDE_DIR - the Tudat include directory.
 #
 #      Original copyright statements (from FindEigen3.cmake:
 #          Copyright (c) 2006, 2007 Montel Laurent, <montel@kde.org>
 #          Copyright (c) 2008, 2009 Gael Guennebaud, <g.gael@free.fr>
 #          Copyright (c) 2009 Benoit Jacob <jacob.benoit.1@gmail.com>
 #
 #      FindEigen3.cmake states that redistribution and use is allowed according to the terms of
 #      the 2-clause BSD license.

macro(_tudat_check_version)
  message(STATUS "Checking for Tudat in: " ${TUDAT_BASE_PATH})
  file(READ "${TUDAT_BASE_PATH}/tudatVersion.h" _tudat_header)

  string(REGEX MATCH "define[ \t]+TUDAT_VERSION_MAJOR[ \t]+([0-9]+)" _tudat_major_version_match "${_tudat_header}")
  set(TUDAT_MAJOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+TUDAT_VERSION_MINOR[ \t]+([0-9]+)" _tudat_minor_version_match "${_tudat_header}")
  set(TUDAT_MINOR_VERSION "${CMAKE_MATCH_1}")

  set(TUDAT_VERSION ${TUDAT_MAJOR_VERSION}.${TUDAT_MINOR_VERSION})
  if(${TUDAT_VERSION} VERSION_LESS ${Tudat_FIND_VERSION})
    set(TUDAT_VERSION_OK FALSE)
  else(${TUDAT_VERSION} VERSION_LESS ${Tudat_FIND_VERSION})
    set(TUDAT_VERSION_OK TRUE)
  endif(${TUDAT_VERSION} VERSION_LESS ${Tudat_FIND_VERSION})

  if(NOT TUDAT_VERSION_OK)

    message(STATUS "Tudat version ${TUDAT_VERSION} found in ${TUDAT_INCLUDE_DIR}, "
                   "but at least version ${Tudat_FIND_VERSION} is required")
  endif(NOT TUDAT_VERSION_OK)

  set(TUDAT_LIBRARIES "tudat")
  set(TUDAT_INCLUDE_DIR ${TUDAT_BASE_PATH}/..)
  set(TUDAT_LIBRARIES_DIR ${TUDAT_BASE_PATH}/../lib)
  link_directories(${TUDAT_LIBRARIES_DIR})
endmacro(_tudat_check_version)

if (TUDAT_BASE_PATH)
  # in cache already
  _tudat_check_version()
  set(TUDAT_FOUND ${TUDAT_VERSION_OK})

else (TUDAT_BASE_PATH)
  find_path(TUDAT_BASE_PATH NAMES tudatVersion.h
      PATHS
      C:
      "$ENV{ProgramFiles}"
      /usr/local
      ${PROJECT_SOURCE_DIR}
      ${PROJECT_SOURCE_DIR}/..
      ${PROJECT_SOURCE_DIR}/../..
      ${PROJECT_SOURCE_DIR}/../../..
      ${PROJECT_SOURCE_DIR}/../../../..
      ${CMAKE_INSTALL_PREFIX}/include
      PATH_SUFFIXES tudat/Tudat
    )

  if(TUDAT_BASE_PATH)
    _tudat_check_version()
  endif(TUDAT_BASE_PATH)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Tudat DEFAULT_MSG TUDAT_BASE_PATH TUDAT_VERSION_OK)

  mark_as_advanced(TUDAT_BASE_PATH)

endif(TUDAT_BASE_PATH)
