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

configure_file(
  ${PROJECT_SOURCE_DIR}/xmlwrapp.pc.in
  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig/xmlwrapp.pc
  @ONLY
)

configure_file(
  ${PROJECT_SOURCE_DIR}/xmlwrapp-config.in
  ${CMAKE_INSTALL_FULL_BINDIR}/xmlwrapp-config
  @ONLY
  FILE_PERMISSIONS
    OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
)

if( XMLWRAPP_WITH_LIBXSLT )
  configure_file(
    ${PROJECT_SOURCE_DIR}/xsltwrapp.pc.in
    ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig/xsltwrapp.pc
    @ONLY
  )
endif( XMLWRAPP_WITH_LIBXSLT )
