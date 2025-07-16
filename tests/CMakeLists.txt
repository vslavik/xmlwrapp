set(TEST_SRCS
  test.h
  test_main.cxx
  attributes/test_attributes.cxx
  document/test_document.cxx
  event/test_event.cxx
  node/test_node.cxx
  tree/test_tree.cxx
  schema/test_schema.cxx
  xpath/test_xpath.cxx
)

set(TEST_DATA_DIRS attributes document event node tree schema xpath)

if(XMLWRAPP_BUILD_XSLTWRAPP)
  LIST(APPEND TEST_SRCS xslt/test_xslt.cxx)
  LIST(APPEND TEST_DATA_DIRS xslt)
endif(XMLWRAPP_BUILD_XSLTWRAPP)

add_executable(test_xmlwrapp ${TEST_SRCS})

target_include_directories(test_xmlwrapp PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(XMLWRAPP_BUILD_XSLTWRAPP)
  target_link_libraries(test_xmlwrapp xsltwrapp_static ${LIBXSLT_LIBRARIES})
endif(XMLWRAPP_BUILD_XSLTWRAPP)


target_link_libraries(test_xmlwrapp ${LIBXML2_LIBRARIES})
target_link_libraries(test_xmlwrapp xmlwrapp_static)

file(COPY ${TEST_DATA_DIRS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} PATTERN "*.cxx" EXCLUDE)

add_test(NAME "test_xmlwrapp" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND test_xmlwrapp)
