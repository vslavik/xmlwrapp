set( XMLWRAPP_SRCS
    libxml/ait_impl.cxx
    libxml/ait_impl.h
    libxml/attributes.cxx
    libxml/cpp11.h
    libxml/document.cxx
    libxml/dtd_impl.cxx
    libxml/dtd_impl.h
    libxml/event_parser.cxx
    libxml/errors.cxx
    libxml/errors_impl.h
    libxml/init.cxx
    libxml/node.cxx
    libxml/nodes_view.cxx
    libxml/node_iterator.cxx
    libxml/node_iterator.h
    libxml/node_manip.cxx
    libxml/node_manip.h
    libxml/pimpl_base.h
    libxml/schema.cxx
    libxml/tree_parser.cxx
    libxml/utility.cxx
    libxml/utility.h
    libxml/xpath.cxx
)

set( XSLTWRAPP_SRCS
    libxslt/init.cxx
    libxslt/result.h
    libxslt/stylesheet.cxx
)

macro(define_libxmlwrapp  arg0  arg1)

    add_library (${arg1} ${arg0} ${XMLWRAPP_SRCS})
    target_include_directories(${arg1} PUBLIC 
                              $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                              $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
    set_target_properties(${arg1} PROPERTIES VERSION 6.0.0 SOVERSION 6 OUTPUT_NAME xmlwrapp)
    install (TARGETS ${arg1} EXPORT XmlwrappTargets DESTINATION lib)

endmacro(define_libxmlwrapp)

define_libxmlwrapp(SHARED xmlwrapp_shared)
define_libxmlwrapp(STATIC xmlwrapp_static)

if( XMLWRAPP_BUILD_XSLTWRAPP )

    macro(define_libxsltwrapp arg0 arg1)
        add_library (${arg1} ${arg0} ${XSLTWRAPP_SRCS})
        target_include_directories(${arg1} PUBLIC 
                              $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                              $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
        install (TARGETS ${arg1} EXPORT XmlwrappTargets DESTINATION lib)
        target_link_libraries(${arg1} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES})
        set_target_properties(${arg1} PROPERTIES VERSION 4.0.0 SOVERSION 4 OUTPUT_NAME xsltwrapp)
    endmacro(define_libxsltwrapp)

    define_libxsltwrapp(SHARED  xsltwrapp_shared)
    define_libxsltwrapp(STATIC  xsltwrapp_static)

endif( XMLWRAPP_BUILD_XSLTWRAPP )
