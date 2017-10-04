add_executable(example03 example.cxx)
target_include_directories(example03 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example03 xmlwrapp_static ${LIBXML2_LIBRARIES})

install(FILES example.cxx DESTINATION share/xmlwrapp/examples/03-xml_generation)
install(TARGETS example03 DESTINATION share/xmlwrapp/examples/03-xml_generation)
