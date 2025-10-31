
##
# documentation for ccutils_define_package function
#
# you can use this function to define packages in your cmake-based project.
#
# this function eventually uses add_library() cmake function.
#
# note: ccutils_define_package is improved create_ccutils_component function.
#       in ccutils_define_package there are no separate SOURCES/HEADERS parameters,
#       but instead ITEMS parameter.
#
# programmer should list items of package. items are essentially names of cpp/hpp files,
# but without extensions. ccutils_define_package take item name and then searches
# named subdirectory for cpp/hpp files with item's name + additioanlly searches cpp/hpp
# files with host cpu/os name. for example, if we assign item 'apple' to package,
# ccutils_define_package will try to find following files:
#   apple.cpp, apple.hpp,
#   apple.any.any.cpp, apple.any.any.hpp,
#   apple.x86_64.any.cpp, apple.x86_64.any.hpp,
#   apple.any.linux-gnu.cpp, apple.any.linux-gnu.hpp,
#   apple.x86_64.linux-gnu.cpp, apple.x86_64.linux-gnu.hpp.
#
# where `x86_64` and `linux-gnu` currently detected host.
#   (this can be used to create separate sources for different CPUs and OSes)
#
# warning: current version of ccutils_define_package is not intended for creation of
#          shared libraries.. maybe in future, but not now.
#
#
# parameters:
#  ---
#  DOMAIN_NAME    - domain name. to decrease name collision
#                   possibility.
#  PROJECT_NAME   - adds additional protection against name collision,
#                   analogicaly to DOMAIN_NAME.
#  PACKAGE_NAME   - name for package to define.
#  SUBDIR         - simply the path to dir in which to search for items, relative to your
#                   CMakeList.txt
#  CPU            - search for those CPU spcecific sources (todo: see function xxx to get valid cpu name)
#  OS             - search for those OS spcecific sources (todo: see function xxx to get valid os name)
#  ITEMS          - list items to package. described abowe.
#  LIBS_PUB       - passed to target_link_libraries() cmake function. todo: explanation
#  LIBS_PRIV      - passed to target_link_libraries() cmake function. todo: explanation
#  INC_DIRS_PUB   - passed to target_include_directories() cmake function. todo: explanation
#  INC_DIRS_PRIV  - passed to target_include_directories() cmake function. todo: explanation
#  ---
#
#  DOMAIN_NAME, PROJECT_NAME - ass you guessed, this way you can define packages
#     for different domains and projects in same cpp code repository and/or same CMakeLists.txt.
#     but you, brobably, to keep orger, should lean to minimize possibility placing
#     different domains and projects in same cpp code repository and/or same CMakeLists.txt.

function(ccutils_define_package)

  set(
    options
    # PRINT_FOUND_FILE_NAMES
  )
  set(
    oneValueArgs
    DOMAIN_NAME
    PROJECT_NAME
    PACKAGE_NAME
    SUBDIR
    CPU
    OS
  )
  set(
    multiValueArgs
    ITEMS
    LIBS_PUB LIBS_PRIV
    INC_DIRS_PUB INC_DIRS_PRIV
  )

  cmake_parse_arguments(
    ccutils_define_package_arg
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  list(SORT ccutils_define_package_arg_ITEMS)

  block()

    # note: this doesn't work, so i forced to write code for each parameter
    # foreach(i DOMAIN_NAME PROJECT_NAME PACKAGE_NAME SUBDIR CPU OS )
    #   set(t_len 0)
    #   set(x "ccutils_define_package_arg_${i}")
    #   set(x1 "${x}")
    #   message(NOTICE "i: ${i}")
    #   message(NOTICE "x: ${x}")
    #   message(NOTICE "x1: ${x1}")
    #   string(LENGTH "${x1}" t_len)
    #   if(t_len EQUAL 0)
    #      message(FATAL_ERROR "${i} must be defined")
    #   endif()
    #  endforeach()

    set(t_len 0)
    string(LENGTH "${ccutils_define_package_arg_DOMAIN_NAME}" t_len)
    if(t_len EQUAL 0)
      message(FATAL_ERROR "DOMAIN_NAME must be defined")
    endif()

    string(LENGTH "${ccutils_define_package_arg_PROJECT_NAME}" t_len)
    if(t_len EQUAL 0)
      message(FATAL_ERROR "PROJECT_NAME must be defined")
    endif()

    string(LENGTH "${ccutils_define_package_arg_PACKAGE_NAME}" t_len)
    if(t_len EQUAL 0)
      message(FATAL_ERROR "PACKAGE_NAME must be defined")
    endif()

    string(LENGTH "${ccutils_define_package_arg_CPU}" t_len)
    if(t_len EQUAL 0)
      message(FATAL_ERROR "CPU must be defined")
    endif()

    string(LENGTH "${ccutils_define_package_arg_OS}" t_len)
    if(t_len EQUAL 0)
      message(FATAL_ERROR "OS must be defined")
    endif()

  endblock()
  # return()

  set(msg "  defining ${ccutils_define_package_arg_PROJECT_NAME} package ")
  string(APPEND msg "${ccutils_define_package_arg_PACKAGE_NAME}")
  message(NOTICE "${msg}")

  add_library(
    ${ccutils_define_package_arg_PACKAGE_NAME}
    EXCLUDE_FROM_ALL
  )

  set(found_sources)
  set(found_headers)

  foreach(item ${ccutils_define_package_arg_ITEMS})

    set(files_found 0)

    set(filename_base_to_check
      "${item}"
      "${item}.any.any"
      "${item}.${ccutils_define_package_arg_CPU}.any"
      "${item}.any.${ccutils_define_package_arg_OS}"
      "${item}.${ccutils_define_package_arg_CPU}.${ccutils_define_package_arg_OS}"
    )

    foreach(base ${filename_base_to_check})

      foreach(ext ".cpp" ".hpp")

        set(filename_base_c)
        string(APPEND filename_base_c "${base}")
        string(APPEND filename_base_c "${ext}")

        # message(NOTICE "filename_base_c: ${filename_base_c}")

        set(fntc "${CMAKE_CURRENT_SOURCE_DIR}/${ccutils_define_package_arg_SUBDIR}/${filename_base_c}")

        # message(NOTICE "fntc: ${fntc}")

        if (EXISTS "${fntc}")

          set(res "")
          set(res_len 0)
          string(REGEX MATCH "^.*\.cpp$" res "${filename_base_c}")
	  string(LENGTH "${res}" res_len)

          # message(NOTICE "res_len GREATER 0: res: ${res}")
          if(res_len GREATER 0)
            list(APPEND found_sources "${fntc}")
            math(EXPR files_found "${files_found} + 1")
          endif()

          set(res "")
          set(res_len 0)
          string(REGEX MATCH "^.*\.hpp$" res "${filename_base_c}")
	  string(LENGTH "${res}" res_len)

          if(res_len GREATER 0)
            list(APPEND found_headers "${fntc}")
            math(EXPR files_found "${files_found} + 1")
          endif()

        endif()
      endforeach()

    endforeach()

    if (${files_found} EQUAL 0)
      message(WARNING "no files found for item '${item}'")
    endif()

  endforeach()

  # print file names

  if (0)
    message("    sources:")
    foreach(i ${found_sources})
      message(NOTICE "      " "${i}")
    endforeach()

    message("    headers:")
    foreach(i ${found_headers})
      message(NOTICE "      " "${i}")
    endforeach()
  endif()

  # final lists formatting

  set(formatted_sources_list)
  foreach(i ${found_sources})
    list(
      APPEND
      formatted_sources_list
      # "${CMAKE_CURRENT_SOURCE_DIR}/${ccutils_define_package_arg_SUBDIR}/${i}"
      "${i}"
    )
  endforeach()


  set(formatted_headers_list)
  foreach(i ${ccutils_define_package_arg_HEADERS})
    list(
      APPEND
      formatted_headers_list
      "${ccutils_define_package_arg_SUBDIR}/includes/${ccutils_define_package_arg_DOMAIN_NAME}/${ccutils_define_package_arg_PACKAGE_NAME}/${ccutils_define_package_arg_SUBDIR}/${i}.hpp"
    )
  endforeach()

  # finalization

  # apply .cpp files
  target_sources(
    ${ccutils_define_package_arg_PACKAGE_NAME}
    PRIVATE
    ${formatted_sources_list}
  )

  # apply .hpp files todo: investigate this
  target_sources(
    ${ccutils_define_package_arg_PACKAGE_NAME}
    PUBLIC
    FILE_SET ${ccutils_define_package_arg_PACKAGE_NAME}_headers
    TYPE HEADERS
    BASE_DIRS ${ccutils_define_package_arg_SUBDIR}/includes
    FILES
    ${formatted_headers_list}
  )

  # add dependencies
  target_link_libraries(
    ${ccutils_define_package_arg_PACKAGE_NAME}
    PUBLIC
    ${ccutils_define_package_arg_LIBS_PUB}
    PRIVATE
    ${ccutils_define_package_arg_LIBS_PRIV}
  )

  # todo: don't remember what's this doing
  target_include_directories(
    ${ccutils_define_package_arg_PACKAGE_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${ccutils_define_package_arg_SUBDIR}/includes>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${ccutils_define_package_arg_DOMAIN_NAME}/${ccutils_define_package_arg_PACKAGE_NAME}/${ccutils_define_package_arg_SUBDIR}>
    PUBLIC
    ${ccutils_define_package_arg_INC_DIRS_PUB}
    PRIVATE
    ${ccutils_define_package_arg_INC_DIRS_PRIV}
  )

  # installation instruction (this is used, for example, if `make install` issued)
  install(
    TARGETS ${ccutils_define_package_arg_PACKAGE_NAME}
    FILE_SET ${ccutils_define_package_arg_PACKAGE_NAME}_headers
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ccutils_define_package_arg_DOMAIN_NAME}/${ccutils_define_package_arg_PACKAGE_NAME}/${ccutils_define_package_arg_SUBDIR}
  )

endfunction()
