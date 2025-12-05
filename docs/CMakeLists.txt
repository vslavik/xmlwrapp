# Prepare configuration options

# See if we can find doxygen, if so, build the docs

#find_program(DOXYGEN doxygen)
find_package(Doxygen)

if(DOXYGEN_FOUND)
  add_custom_target(xmlwrapp_docs ALL DEPENDS html/index.html)
  add_custom_command(
    OUTPUT
      html/index.html
    COMMAND
      ${CMAKE_COMMAND} -E env
        DOXYGEN_OUTPUT_DIRECTORY_SLASH=${CMAKE_CURRENT_BINARY_DIR}/
        ${DOXYGEN_EXECUTABLE} Doxyfile
    MAIN_DEPENDENCY Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION ${CMAKE_INSTALL_DOCDIR})
else()
  message(WARNING "Doxygen not found, documentation will not be built.")
  set(XMLWRAPP_DOC OFF PARENT_SCOPE)
endif(DOXYGEN_FOUND)
