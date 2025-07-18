# Configure the given library to use hidden visibility.
#
# Note that this does nothing when shared libraries are not used.
function(use_hidden_visibility target)
  if (XMLWRAPP_SHARED)
    # Construct XMLWRAPP_USE_DLL or XSLTWRAPP_USE_DLL symbol name.
    string(TOUPPER "${target}_USE_DLL" use_dll_symbol)
    target_compile_definitions(${target}
      PRIVATE
        HAVE_VISIBILITY
      INTERFACE
        ${use_dll_symbol}
    )
    set_target_properties(${target} PROPERTIES
      DEFINE_SYMBOL "DLL_EXPORT"
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
    )
  endif()
endfunction()

add_library(xmlwrapp ${XMLWRAPP_LIB_TYPE})
target_sources(xmlwrapp
  PRIVATE
    libxml/ait_impl.cxx
    libxml/ait_impl.h
    libxml/attributes.cxx
    libxml/document.cxx
    libxml/dtd_impl.cxx
    libxml/dtd_impl.h
    libxml/errors.cxx
    libxml/errors_impl.h
    libxml/event_parser.cxx
    libxml/init.cxx
    libxml/node.cxx
    libxml/node_iterator.cxx
    libxml/node_iterator.h
    libxml/node_manip.cxx
    libxml/node_manip.h
    libxml/nodes_view.cxx
    libxml/relaxng.cxx
    libxml/schema.cxx
    libxml/tree_parser.cxx
    libxml/utility.cxx
    libxml/utility.h
    libxml/version.cxx
    libxml/xpath.cxx
)

target_include_directories(xmlwrapp
  PRIVATE
    ${LIBXML2_INCLUDE_DIRS}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_options(xmlwrapp
  PUBLIC
    ${LIBXML2_LDFLAGS}
)
target_link_libraries(xmlwrapp
  PUBLIC
    ${LIBXML2_LIBRARIES}
)
set_target_properties(xmlwrapp PROPERTIES VERSION 6.0.0 SOVERSION 6)
use_hidden_visibility(xmlwrapp)
install (TARGETS xmlwrapp EXPORT XmlwrappTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

if( XMLWRAPP_WITH_LIBXSLT )
  add_library(xsltwrapp ${XMLWRAPP_LIB_TYPE})
  target_sources(xsltwrapp
    PRIVATE
      libxslt/init.cxx
      libxslt/stylesheet.cxx
      libxslt/result.h
  )
  target_include_directories(xsltwrapp
    PRIVATE
      ${LIBXSLT_INCLUDE_DIRS}
      ${LIBEXSLT_INCLUDE_DIRS}
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  target_link_options(xsltwrapp
    PUBLIC
      ${LIBXSLT_LDFLAGS}
      ${LIBEXSLT_LDFLAGS}
  )
  target_link_libraries(xsltwrapp
    PUBLIC
      xmlwrapp
      ${LIBXSLT_LIBRARIES}
      ${LIBEXSLT_LIBRARIES}
  )
  set_target_properties(xsltwrapp PROPERTIES VERSION 4.0.0 SOVERSION 4)
  use_hidden_visibility(xsltwrapp)
  install (TARGETS xsltwrapp EXPORT XmlwrappTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif( XMLWRAPP_WITH_LIBXSLT )
