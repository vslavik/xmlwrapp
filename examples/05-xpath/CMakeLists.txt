add_executable(example05 example.cxx)
target_include_directories(example05 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example05 xmlwrapp)
