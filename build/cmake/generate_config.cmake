# This script is used by the main CMakeLists.txt to generate pkg-config .pc
# files and xmlwrapp-config script during the install time.

# Set the variables with the same names as those used in the .in files using
# the variables predefined for us by the main script.

set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${prefix})
set(libdir ${CMAKE_INSTALL_FULL_LIBDIR})
set(includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set(VERSION ${XMLWRAPP_VERSION})

set(XML2_LIBS "")
foreach(p ${LIBXML2_LIBRARY_DIRS})
    string(APPEND XML2_LIBS " -L${p}")
endforeach()
foreach(lib ${LIBXML2_LIBRARIES})
    string(APPEND XML2_LIBS " -l${lib}")
endforeach()

# This is another variable which must have this name because this is what is
# used in xmlwrapp-config.in.
set(LEGACY_LINK_FLAGS "-lxmlwrapp ${XML2_LIBS}")

if( XMLWRAPP_WITH_LIBXSLT )
  set(XSLT_LIBS "")
  foreach(p ${LIBXSLT_LIBRARY_DIRS})
    string(APPEND XSLT_LIBS " -L${p}")
  endforeach()
  foreach(lib ${LIBXSLT_LIBRARIES})
    string(APPEND XSLT_LIBS " -l${lib}")
  endforeach()
  string(APPEND LEGACY_LINK_FLAGS " -lxsltwrapp ${XSLT_LIBS}")
endif( XMLWRAPP_WITH_LIBXSLT )

string(STRIP ${LEGACY_LINK_FLAGS} LEGACY_LINK_FLAGS)

# Function to configure a file if it is necessary, i.e. if it doesn't exist or
# its contents has changed.
#
# This outputs the same messages as CMake install(), making the installation of
# the config files here consistent with all the other ones.
function(configure_file_if_necessary input_file output_file make_executable)
  # Create temporary file
  set(temp_file "${output_file}.tmp")
  set(configure_args "@ONLY")
  if(make_executable)
      list(APPEND configure_args "FILE_PERMISSIONS")
      list(APPEND configure_args "OWNER_EXECUTE")
      list(APPEND configure_args "OWNER_WRITE")
      list(APPEND configure_args "OWNER_READ")
      list(APPEND configure_args "GROUP_EXECUTE")
      list(APPEND configure_args "GROUP_READ")
      list(APPEND configure_args "WORLD_EXECUTE")
      list(APPEND configure_args "WORLD_READ")
  endif()
  configure_file("${input_file}" "${temp_file}" ${configure_args})

  # Check if we need to update
  set(needs_update TRUE)

  if(EXISTS "${output_file}")
      file(READ "${output_file}" existing_content)
      file(READ "${temp_file}" new_content)

      if("${existing_content}" STREQUAL "${new_content}")
          set(needs_update FALSE)
          message(STATUS "Up-to-date: ${output_file}")
      endif()
  endif()

  if(needs_update)
      message(STATUS "Installing: ${output_file}")
      file(RENAME "${temp_file}" "${output_file}")
  else()
      file(REMOVE "${temp_file}")
  endif()
endfunction()

configure_file_if_necessary(
  ${PROJECT_SOURCE_DIR}/xmlwrapp.pc.in
  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig/xmlwrapp.pc
  OFF
)

configure_file_if_necessary(
  ${PROJECT_SOURCE_DIR}/xmlwrapp-config.in
  ${CMAKE_INSTALL_FULL_BINDIR}/xmlwrapp-config
  ON
)

if( XMLWRAPP_WITH_LIBXSLT )
  configure_file_if_necessary(
    ${PROJECT_SOURCE_DIR}/xsltwrapp.pc.in
    ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig/xsltwrapp.pc
    OFF
  )
endif( XMLWRAPP_WITH_LIBXSLT )
