add_executable(example04 example.cxx)
target_include_directories(example04 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example04 xsltwrapp)

install(FILES example.cxx example.xml example.xsl DESTINATION share/xmlwrapp/examples/04-xslt)
install(TARGETS example04 DESTINATION share/xmlwrapp/examples/04-xslt)
