# Prepare configuration options

# See if we can find doxygen, if so, build the docs

#find_program(DOXYGEN doxygen)
find_package(Doxygen)

if(DOXYGEN_FOUND)
  file(COPY Doxyfile manual DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  add_custom_target(xmlwrapp_docs ALL DEPENDS html)
  add_custom_command(OUTPUT html
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile > /dev/null 2>&1
    MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION ${CMAKE_INSTALL_DOCDIR})
else()
  message(WARNING "Doxygen not found, documentation will not be built.")
  set(XMLWRAPP_DOC OFF PARENT_SCOPE)
endif(DOXYGEN_FOUND)
