add_executable(example01 example.cxx)
target_include_directories(example01 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example01 xmlwrapp)
