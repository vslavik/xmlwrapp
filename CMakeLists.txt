# 3.20 is required due to the of configure_file(FILE_PERMISSIONS).
cmake_minimum_required(VERSION 3.20)

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

# These options are enabled by default when building standalone library but
# disabled when using it via add_subdirectory() from another project.
option(XMLWRAPP_DOC "Build and install the documentation (requires Doxygen)" ${PROJECT_IS_TOP_LEVEL})
option(XMLWRAPP_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})
option(XMLWRAPP_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})

# This is always disabled but can be disabled by the super-project if it is
# not needed before calling add_subdirectory().
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


# Delegate to another script for pkg-config and xmlwrapp-config creation as
# the paths used in them are only known at install time and not at configure
# time (installtion prefix can be changed when using "cmake --install").
#
# CMAKE_SYSTEM_NAME and CMAKE_SIZEOF_VOID_P must be set to avoid warning from
# GNUInstallDirs, we can do it as their values won't change between configure
# and install time.
install(CODE "
  set(CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
  set(CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
  include(GNUInstallDirs)
  set(XMLWRAPP_VERSION \"${XMLWRAPP_VERSION}\")
  set(XMLWRAPP_WITH_LIBXSLT \"${XMLWRAPP_WITH_LIBXSLT}\")
  set(LIBXML2_LIBRARY_DIRS \"${LIBXML2_LIBRARY_DIRS}\")
  set(LIBXML2_LIBRARIES \"${LIBXML2_LIBRARIES}\")
  set(LIBXSLT_LIBRARY_DIRS \"${LIBXSLT_LIBRARY_DIRS}\")
  set(LIBXSLT_LIBRARIES \"${LIBXSLT_LIBRARIES}\")
  set(PROJECT_SOURCE_DIR \"${PROJECT_SOURCE_DIR}\")
  include(\"${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/generate_config.cmake\")
")


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
  build/cmake/XmlwrappConfig.cmake.in
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
set(CPACK_SOURCE_GENERATOR "TGZ;TXZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "xmlwrapp-${XMLWRAPP_VERSION}")

set(CPACK_SOURCE_INSTALLED_DIRECTORIES
  "${CMAKE_SOURCE_DIR};/;${CMAKE_BINARY_DIR}/docs/html;/docs/html"
)
set(CPACK_SOURCE_IGNORE_FILES
  "[.]swp\$"
  "autom4te[.]cache"
  "/[.]git"
)
include(CPack)

# Create a custom target to ensure that we build the docs that must be
# included in the package.
add_custom_target(dist
  COMMAND "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --target package_source
  DEPENDS xmlwrapp_docs
  COMMENT "Creating source package."
  VERBATIM
)

# Create another custom target to check that the created package works.
add_custom_target(distcheck
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E remove_directory xmlwrapp-${XMLWRAPP_VERSION} distcheck-build distcheck-install
  COMMAND ${CMAKE_COMMAND} -E tar xzf ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz
  COMMAND ${CMAKE_COMMAND} -S xmlwrapp-${XMLWRAPP_VERSION} -B distcheck-build -DCMAKE_BUILD_TYPE=Release
  COMMAND ${CMAKE_COMMAND} --build distcheck-build
  COMMAND ${CMAKE_COMMAND} --build distcheck-build --target test
  COMMAND ${CMAKE_COMMAND} --install distcheck-build --prefix distcheck-install
  COMMAND ${CMAKE_COMMAND} -E echo "Package xmlwrapp-${XMLWRAPP_VERSION} is ready."
  DEPENDS dist
  COMMENT "Checking that the source package is usable."
  VERBATIM
)

# Give build summary at the end.
if (CMAKE_BUILD_TYPE)
  set(XMLWRAPP_CONFIG_DESCRIPTION " in ${CMAKE_BUILD_TYPE} configuration")
endif()
message(STATUS "xmlwrapp ${XMLWRAPP_VERSION} will be built as ${XMLWRAPP_LIB_TYPE} library${XMLWRAPP_CONFIG_DESCRIPTION}.")
message(STATUS "  Build xsltwrapp: ${XMLWRAPP_WITH_LIBXSLT}")
message(STATUS "  Documentation:   ${XMLWRAPP_DOC}")
message(STATUS "  Examples:        ${XMLWRAPP_EXAMPLES}")
message(STATUS "  Tests:           ${XMLWRAPP_TESTS}")
