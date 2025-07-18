add_executable(example02 example.cxx)
target_include_directories(example02 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example02 xmlwrapp)
