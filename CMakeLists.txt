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

set(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(INSTALL_DOC_DIR ${CMAKE_INSTALL_PREFIX}/share/xmlwrapp/doc)

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/xmlwrapp)
endif(WIN32 AND NOT CYGWIN)
set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/${DEF_INSTALL_CMAKE_DIR})

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
  #TODO install examples in <prefix>/share/xmlwrapp/examples/01-.. 02-.. etc
  add_subdirectory(examples)
endif( XMLWRAPP_EXAMPLES )

if( XMLWRAPP_TESTS )
  include(CTest)
  add_subdirectory(tests)
endif( XMLWRAPP_TESTS)

if( XMLWRAPP_DOC )
  #TODO build and install docs in <prefix>/share/xmlwrapp/doc/html/
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
  install (FILES "${binarydir}/xmlwrapp.pc" DESTINATION lib/pkgconfig)

  configure_file( "xmlwrapp-config.in" "xmlwrapp-config" @ONLY)
  install (PROGRAMS "${binarydir}/xmlwrapp-config"
           DESTINATION bin)

  if( XMLWRAPP_WITH_LIBXSLT )
    configure_file( "xsltwrapp.pc.in" "xsltwrapp.pc" @ONLY)
    install (FILES "${binarydir}/xsltwrapp.pc" DESTINATION lib/pkgconfig)
  endif( XMLWRAPP_WITH_LIBXSLT )

endfunction()

config_pkgconf("${CMAKE_INSTALL_PREFIX}"
               "${INSTALL_LIBRARY_DIR}"
               "${INSTALL_INCLUDE_DIR}"
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

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
file(RELATIVE_PATH REL_LIBRARY_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_LIBRARY_DIR}")

set(CONF_INCLUDE_DIR "\${XMLWRAPP_CMAKE_DIR}/${REL_INCLUDE_DIR}")
set(CONF_LIBRARY_DIR "\${XMLWRAPP_CMAKE_DIR}/${REL_LIBRARY_DIR}")

configure_package_config_file(
  XmlwrappConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/XmlwrappConfig.cmake
  INSTALL_DESTINATION
    ${INSTALL_CMAKE_DIR}
)

install(
  FILES
    ${PROJECT_BINARY_DIR}/XmlwrappConfig.cmake
    ${PROJECT_BINARY_DIR}/XmlwrappConfigVersion.cmake
  DESTINATION
    ${INSTALL_CMAKE_DIR}
  COMPONENT
    dev
)

install(EXPORT XmlwrappTargets DESTINATION "${DEF_INSTALL_CMAKE_DIR}" COMPONENT dev)



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
