add_executable(example05 example.cxx)
target_include_directories(example05 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example05 xmlwrapp)

install(FILES example.cxx example.xml DESTINATION share/xmlwrapp/examples/05-xpath)
install(TARGETS example05 DESTINATION share/xmlwrapp/examples/05-xpath)
