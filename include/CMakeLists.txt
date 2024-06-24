set(XMLWRAPP_HEADERS 
    xmlwrapp/attributes.h
    xmlwrapp/_cbfo.h
    xmlwrapp/document.h
    xmlwrapp/event_parser.h
    xmlwrapp/errors.h
    xmlwrapp/export.h
    xmlwrapp/init.h
    xmlwrapp/node.h
    xmlwrapp/nodes_view.h
    xmlwrapp/schema.h
    xmlwrapp/tree_parser.h
    xmlwrapp/version.h
    xmlwrapp/xmlwrapp.h
    xmlwrapp/xpath.h
)

set(XSLTWRAPP_HEADERS 
    xsltwrapp/init.h
    xsltwrapp/stylesheet.h
    xsltwrapp/xsltwrapp.h
)

install (FILES ${XMLWRAPP_HEADERS} DESTINATION include/xmlwrapp) 

if( XMLWRAPP_BUILD_XSLTWRAPP )
    install (FILES ${XSLTWRAPP_HEADERS} DESTINATION include/xsltwrapp)
endif( XMLWRAPP_BUILD_XSLTWRAPP )
