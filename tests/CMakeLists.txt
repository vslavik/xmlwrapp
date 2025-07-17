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

if(XMLWRAPP_WITH_LIBXSLT)
  LIST(APPEND TEST_SRCS xslt/test_xslt.cxx)
  LIST(APPEND TEST_DATA_DIRS xslt)
endif(XMLWRAPP_WITH_LIBXSLT)

add_executable(test_xmlwrapp ${TEST_SRCS})

target_include_directories(test_xmlwrapp PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(XMLWRAPP_WITH_LIBXSLT)
  target_link_libraries(test_xmlwrapp xsltwrapp)
else()
  target_link_libraries(test_xmlwrapp xmlwrapp)
endif(XMLWRAPP_WITH_LIBXSLT)

add_test(NAME "test_xmlwrapp" COMMAND test_xmlwrapp)

# Allow test must to find the data files (alternative could be to run it in
# this directory using WORKING_DIRECTORY in add_test() above, but this seems
# better because it's more explicit).
set_tests_properties(test_xmlwrapp
  PROPERTIES
    ENVIRONMENT "srcdir=${CMAKE_CURRENT_SOURCE_DIR}"
)
