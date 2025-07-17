add_executable(example01 example.cxx)
target_include_directories(example01 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example01 xmlwrapp)

install(FILES example.cxx DESTINATION share/xmlwrapp/examples/01-tree_parsing)
install(TARGETS example01 DESTINATION share/xmlwrapp/examples/01-tree_parsing)
