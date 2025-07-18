add_executable(example03 example.cxx)
target_include_directories(example03 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example03 xmlwrapp)
