add_executable(example04 example.cxx)
target_include_directories(example04 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example04 xsltwrapp)
