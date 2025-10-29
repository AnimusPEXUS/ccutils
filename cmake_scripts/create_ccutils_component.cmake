
##
# documentation for create_ccutils_component function
#
# this function used for creating components of ccutils.
# components created by using add_library() cmake function.
#
# parameters:
#  TARGETNAME     - name for component, for example 'ccutils_xxx' where xxx is desired
#                   name and whole 'ccutils_xxx' is to be addressed by othe components.
#  SUBDIR         - subdir in ccutils sources directory in which source files for
#                   'ccutils_xxx' is found.
#  SOURCES        - names of files to be used as sources (without .c/.cpp or other
#                   extensions)
#  HEADERS        - same as SOURCES, but for header files.
#  LIBS_PUB       - passed to target_link_libraries() cmake function
#  LIBS_PRIV      - passed to target_link_libraries() cmake function
#  INC_DIRS_PUB   - passed to target_include_directories() cmake function
#  INC_DIRS_PRIV  - passed to target_include_directories() cmake function

function(create_ccutils_component)

  set(options)
  set(oneValueArgs TARGETNAME SUBDIR)
  set(
    multiValueArgs
    SOURCES HEADERS
    LIBS_PUB LIBS_PRIV
    INC_DIRS_PUB INC_DIRS_PRIV
  )

  cmake_parse_arguments(
    create_ccutils_component
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  list(SORT create_ccutils_component_SOURCES)
  list(SORT create_ccutils_component_HEADERS)

  message("defining ccutils component: ${create_ccutils_component_TARGETNAME}")

  # todo: make configurable option for EXCLUDE_FROM_ALL parameter in add_library

  add_library(
    ${create_ccutils_component_TARGETNAME}
    EXCLUDE_FROM_ALL
  )

  set(formatted_sources_list)
  foreach(i ${create_ccutils_component_SOURCES})
    list(
      APPEND
      formatted_sources_list
      "${CMAKE_CURRENT_SOURCE_DIR}/${create_ccutils_component_SUBDIR}/${i}.cpp"
    )

    set(nfn "${CMAKE_CURRENT_SOURCE_DIR}/${create_ccutils_component_SUBDIR}/${i}.${PARTICULAR_SYSTEM_NAME}.cpp")
    if (EXISTS ${nfn})
      list(
	APPEND
	formatted_sources_list
	"${nfn}"
      )
    endif()

  endforeach()

  target_sources(
    ${create_ccutils_component_TARGETNAME}
    PRIVATE
    ${formatted_sources_list}
  )

  set(formatted_headers_list)
  foreach(i ${create_ccutils_component_HEADERS})
    list(
      APPEND
      formatted_headers_list
      "${create_ccutils_component_SUBDIR}/includes/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}/${i}.hpp"
    )


    set(nfn "${create_ccutils_component_SUBDIR}/includes/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}/${i}.${PARTICULAR_SYSTEM_NAME}.hpp")
    if (EXISTS ${nfn})
      list(
	APPEND
	formatted_headers_list
	"${nfn}"
      )
    endif()

  endforeach()


  target_sources(
    ${create_ccutils_component_TARGETNAME}
    PUBLIC
    FILE_SET ${create_ccutils_component_TARGETNAME}_headers
    TYPE HEADERS
    BASE_DIRS ${create_ccutils_component_SUBDIR}/includes
    FILES
    ${formatted_headers_list}
  )

  target_link_libraries(
    ${create_ccutils_component_TARGETNAME}
    PUBLIC
    ${create_ccutils_component_LIBS_PUB}
    PRIVATE
    ${create_ccutils_component_LIBS_PRIV}
  )

  target_include_directories(
    ${create_ccutils_component_TARGETNAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${create_ccutils_component_SUBDIR}/includes>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}>
    PUBLIC
    ${create_ccutils_component_INC_DIRS_PUB}
    PRIVATE
    ${create_ccutils_component_INC_DIRS_PRIV}
  )

  install(
    TARGETS ${create_ccutils_component_TARGETNAME}
    FILE_SET ${create_ccutils_component_TARGETNAME}_headers
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}
  )


endfunction()

