cmake_minimum_required(VERSION 3.16)

# Extract version from the header which is the source of truth for it.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/xmlwrapp/version.h" VERSION_H_CONTENT)
string(REGEX MATCH "#define +XMLWRAPP_VERSION_MAJOR +([0-9]+)" _ "${VERSION_H_CONTENT}")
set(XMLWRAPP_VERSION_MAJOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define +XMLWRAPP_VERSION_MINOR +([0-9]+)" _ "${VERSION_H_CONTENT}")
set(XMLWRAPP_VERSION_MINOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define +XMLWRAPP_VERSION_MICRO +([0-9]+)" _ "${VERSION_H_CONTENT}")
set(XMLWRAPP_VERSION_PATCH "${CMAKE_MATCH_1}")

set(XMLWRAPP_VERSION "${XMLWRAPP_VERSION_MAJOR}.${XMLWRAPP_VERSION_MINOR}.${XMLWRAPP_VERSION_PATCH}")

project(xmlwrapp
  VERSION ${XMLWRAPP_VERSION}
  DESCRIPTION "C++ library for working with XML data, built atop the venerable libxml2 C library."
  LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# Decide whether to build xmlwrapp as static or shared library: this can be
# controlled by it specifically by setting XMLWRAPP_SHARED option and inherits
# the value of the standard BUILD_SHARED_LIBS option if not set.
if(DEFINED BUILD_SHARED_LIBS)
  set(XMLWRAPP_SHARED_DEFAULT ${BUILD_SHARED_LIBS})
else()
  set(XMLWRAPP_SHARED_DEFAULT ON)
endif()
option(XMLWRAPP_SHARED "Build using shared libraries" ${XMLWRAPP_SHARED_DEFAULT})

if (XMLWRAPP_SHARED)
  set(XMLWRAPP_LIB_TYPE "SHARED")
else()
  set(XMLWRAPP_LIB_TYPE "STATIC")
endif()

option(XMLWRAPP_DOC "Build and install the documentation (requires Doxygen)" ON)
option(XMLWRAPP_EXAMPLES "Build examples" ON)
option(XMLWRAPP_TESTS "Build test" ON)
option(XMLWRAPP_WITH_LIBXSLT "Build libxsltwrapp library" ON)

include(GNUInstallDirs)
set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/xmlwrapp)

set(CMAKE_CXX_COMPILER_VISIBILITY_PRESET hidden)

find_package(PkgConfig)

pkg_search_module(LIBXML2 REQUIRED libxml-2.0 >= 2.4.28)

if( XMLWRAPP_WITH_LIBXSLT )
  pkg_search_module(LIBXSLT REQUIRED libxslt >= 1.1.6)
  pkg_search_module(LIBEXSLT REQUIRED libexslt)
endif( XMLWRAPP_WITH_LIBXSLT )


add_subdirectory(include)
add_subdirectory(src)

if( XMLWRAPP_EXAMPLES )
  add_subdirectory(examples)
endif( XMLWRAPP_EXAMPLES )

if( XMLWRAPP_TESTS )
  include(CTest)
  add_subdirectory(tests)
endif( XMLWRAPP_TESTS)

if( XMLWRAPP_DOC )
  add_subdirectory(docs)
endif( XMLWRAPP_DOC )


#----------------------------------------------
# pkg-config files use autoconf variable names
# placed in function to keep project namespace free of autoconf build sys var names
function(config_pkgconf
  prefix
  libdir
  includedir
  binarydir
  VERSION
  xml2dirs
  xml2libs
  xsltdirs
  xsltlibs
)

  set(exec_prefix ${prefix})
  set(XML2_LIBS "")
  foreach(p ${xml2dirs})
      list(APPEND XML2_LIBS "-L${p}")
  endforeach()
  foreach(lib ${xml2libs})
      list(APPEND XML2_LIBS "-l${lib}")
  endforeach()

  set(LEGACY_LINK_FLAGS "-lxmlwrapp ${XML2_LIBS}")

  if( XMLWRAPP_WITH_LIBXSLT )
    set(XSLT_LIBS "")
    foreach(p ${xsltdirs})
      list(APPEND XSLT_LIBS "-L${p}")
    endforeach()
    foreach(lib ${xsltlibs})
      list(APPEND XSLT_LIBS "-l${lib}")
    endforeach()
    list(APPEND LEGACY_LINK_FLAGS "-lxsltwrapp ${XSLT_LIBS}")
  endif( XMLWRAPP_WITH_LIBXSLT )

  configure_file( "xmlwrapp.pc.in" "xmlwrapp.pc" @ONLY)
  install (FILES "${binarydir}/xmlwrapp.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

  configure_file( "xmlwrapp-config.in" "xmlwrapp-config" @ONLY)
  install (PROGRAMS "${binarydir}/xmlwrapp-config"
           DESTINATION ${CMAKE_INSTALL_BINDIR})

  if( XMLWRAPP_WITH_LIBXSLT )
    configure_file( "xsltwrapp.pc.in" "xsltwrapp.pc" @ONLY)
    install (FILES "${binarydir}/xsltwrapp.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
  endif( XMLWRAPP_WITH_LIBXSLT )

endfunction()

config_pkgconf("${CMAKE_INSTALL_PREFIX}"
               "${CMAKE_INSTALL_LIBDIR}"
               "${CMAKE_INSTALL_INCLUDEDIR}"
               "${PROJECT_BINARY_DIR}"
               "${XMLWRAPP_VERSION}"
               "${LIBXMLS2_LIBRARY_DIRS}"
               "${LIBXML2_LIBRARIES}"
               "${LIBXSLT_LIBRARY_DIRS}"
               "${LIBXSLT_LIBRARIES}"
               )
#----------------------------------------------



# add all targets to build-tree export set
export(TARGETS xmlwrapp FILE "${PROJECT_BINARY_DIR}/XmlwrappTargets.cmake")
if( XMLWRAPP_WITH_LIBXSLT )
  export(TARGETS xsltwrapp APPEND FILE "${PROJECT_BINARY_DIR}/XmlwrappTargets.cmake")
endif( XMLWRAPP_WITH_LIBXSLT )

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE Xmlwrapp)

# Generate config file for find_package(xmlwrapp).
include (CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/XmlwrappConfigVersion.cmake"
  COMPATIBILITY SameMinorVersion
)

configure_package_config_file(
  XmlwrappConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/XmlwrappConfig.cmake
  PATH_VARS
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
  INSTALL_DESTINATION
    ${INSTALL_CMAKE_DIR}
  # We don't use components, although we probably should for xsltwrapp.
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
  FILES
    ${PROJECT_BINARY_DIR}/XmlwrappConfig.cmake
    ${PROJECT_BINARY_DIR}/XmlwrappConfigVersion.cmake
  DESTINATION
    ${INSTALL_CMAKE_DIR}
)

install(EXPORT XmlwrappTargets DESTINATION ${INSTALL_CMAKE_DIR})



set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "xmlwrapp is a C++ library for working with XML data, built atop the venerable libxml2 C library.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${XMLWRAPP_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${XMLWRAPP_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${XMLWRAPP_VERSION_PATCH})

set(CPACK_SOURCE_IGNORE_FILES
  "~$"
  "^.gitignore$"
  "^*.git"
)
include(CPack)

# Give build summary at the end.
if (CMAKE_BUILD_TYPE)
  set(XMLWRAPP_CONFIG_DESCRIPTION " in ${CMAKE_BUILD_TYPE} configuration")
endif()
message(STATUS "xmlwrapp ${XMLWRAPP_VERSION} will be built as ${XMLWRAPP_LIB_TYPE} library${XMLWRAPP_CONFIG_DESCRIPTION}.")
message(STATUS "  Build xsltwrapp: ${XMLWRAPP_WITH_LIBXSLT}")
message(STATUS "  Documentation:   ${XMLWRAPP_DOC}")
message(STATUS "  Examples:        ${XMLWRAPP_EXAMPLES}")
message(STATUS "  Tests:           ${XMLWRAPP_TESTS}")
