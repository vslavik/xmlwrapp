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
    libxml/xpath.cxx
)

target_include_directories(xmlwrapp
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
target_link_libraries(xmlwrapp
  PUBLIC
    ${LIBXML2_LIBRARIES}
)
set_target_properties(xmlwrapp PROPERTIES VERSION 6.0.0 SOVERSION 6)
install (TARGETS xmlwrapp EXPORT XmlwrappTargets DESTINATION lib)

if( XMLWRAPP_BUILD_XSLTWRAPP )
  add_library(xsltwrapp ${XMLWRAPP_LIB_TYPE})
  target_sources(xsltwrapp
    PRIVATE
      libxslt/init.cxx
      libxslt/stylesheet.cxx
      libxslt/result.h
  )
  target_include_directories(xsltwrapp
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
  )
  target_link_libraries(xsltwrapp
    PUBLIC
      xmlwrapp
      ${LIBXSLT_LIBRARIES}
      ${LIBEXSLT_LIBRARIES}
  )
  set_target_properties(xsltwrapp PROPERTIES VERSION 4.0.0 SOVERSION 4)
  install (TARGETS xsltwrapp EXPORT XmlwrappTargets DESTINATION lib)
endif( XMLWRAPP_BUILD_XSLTWRAPP )
