cmake_minimum_required(VERSION 3.27)

project(ccutils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# system detection. this is used to select/provide system-specific sources and headers

set(PARTICULAR_SYSTEM_NAME "")
# todo: detection should be improver here in future
if (((DEFINED UNIX) AND (UNIX EQUAL 1)) AND ((DEFINED LINUX) AND (LINUX EQUAL 1)))
  set(PARTICULAR_SYSTEM_NAME "linux-gnu")
else()
  message(FATAL_ERROR "system not supported")
endif()


#find_package(PkgConfig REQUIRED)

# set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_DEBUG_LIBS        OFF)
#set(Boost_USE_RELEASE_LIBS       ON)
#set(Boost_USE_MULTITHREADED      ON)

#find_package(Boost REQUIRED COMPONENTS json regex)



function(create_ccutils_component)
  set(options)
  set(oneValueArgs TARGETNAME SUBDIR)
  set(multiValueArgs
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

  message("  adding component: ${create_ccutils_component_TARGETNAME}")

  add_library(
    ${create_ccutils_component_TARGETNAME}
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

create_ccutils_component(
  TARGETNAME ccutils_utils_cf
  SUBDIR utils

  SOURCES
  cf

  HEADERS
  cf

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
)

create_ccutils_component(
  TARGETNAME ccutils_utils_concepts
  SUBDIR utils

  SOURCES
  concepts

  HEADERS
  concepts

  LIBS_PUB
)


create_ccutils_component(
  TARGETNAME ccutils_errors
  SUBDIR errors

  SOURCES
  e

  HEADERS
  e

  LIBS_PUB
  ccutils_unicode
)


set(
  CCUTILS_UNICODE_BACKEND
  icu
  CACHE
  STRING
  "set to 'icu' or 'ccutils' - select backend used by ccutils_unicode library"
)

block(SCOPE_FOR VARIABLES)
  set(CCUTILS_UNICODE_BACKEND_correct OFF)
  foreach(i IN ITEMS icu ccutils)
    if ("${i}" STREQUAL "${CCUTILS_UNICODE_BACKEND}")
      set(CCUTILS_UNICODE_BACKEND_correct ON)
      break()
    endif()
  endforeach()
  if (NOT CCUTILS_UNICODE_BACKEND_correct)
    message(FATAL_ERROR "invalid value for CCUTILS_UNICODE_BACKEND")
  endif()
endblock()

message("CCUTILS_UNICODE_BACKEND is ${CCUTILS_UNICODE_BACKEND}")

set(
  CCUTILS_UNICODE_SOURCES
  u
)

set(
  CCUTILS_UNICODE_HEADERS
  u
)

set(
  CCUTILS_UNICODE_LIBS_PUB
  ccutils_errors
  ccutils_regexp
  ccutils_repr
  ccutils_utils_cf
)


#set(
#  CCUTILS_UNICODE_LIBS_PUB
#  ccutils_errors
#  ccutils_regexp
#  ccutils_repr
#)

set(
  CCUTILS_UNICODE_INC_DIRS_PUB
  ${ccutils_errors_INCLUDE_DIRS}
)

if (CCUTILS_UNICODE_BACKEND STREQUAL icu)

  #set(ICU_DEBUG ON)

  find_package(ICU 73.0 COMPONENTS uc i18n io REQUIRED)

  create_ccutils_component(
    TARGETNAME ccutils_unicode_backend_icu
    SUBDIR unicode_backend_icu

    SOURCES
    u_backend

    HEADERS
    u_backend

    LIBS_PUB
    ICU::uc
    ICU::i18n
    ICU::io
    ccutils_errors
    ccutils_unicode

    INC_DIRS_PUB
    ICU::uc
    ICU::i18n
    ICU::io
  )

  list(
    APPEND
    CCUTILS_UNICODE_SOURCES
  )
  list(
    APPEND
    CCUTILS_UNICODE_HEADERS
  )
  list(
    APPEND
    CCUTILS_UNICODE_LIBS_PUB
    ccutils_unicode_backend_icu
  )
  list(
    APPEND
    CCUTILS_UNICODE_INC_DIRS_PUB
  )

endif()

if (CCUTILS_UNICODE_BACKEND STREQUAL ccutils)

  create_ccutils_component(
    TARGETNAME ccutils_unicode_backend_ccutils
    SUBDIR unicode_backend_ccutils

    SOURCES
    u_backend
    ccuub_ccutils_generated_types
    ccuub_ccutils_generated_char_db

    HEADERS
    u_backend
    ccuub_ccutils_generated_types
    ccuub_ccutils_generated_char_db

    LIBS_PUB
    ccutils_errors
    ccutils_unicode

    INC_DIRS_PUB
  )

  list(
    APPEND
    CCUTILS_UNICODE_SOURCES
  )
  list(
    APPEND
    CCUTILS_UNICODE_HEADERS
  )
  list(
    APPEND
    CCUTILS_UNICODE_LIBS_PUB
    ccutils_unicode_backend_ccutils
  )
  list(
    APPEND
    CCUTILS_UNICODE_INC_DIRS_PUB
  )
endif()


create_ccutils_component(
  TARGETNAME ccutils_unicode
  SUBDIR unicode

  SOURCES
  ${CCUTILS_UNICODE_SOURCES}

  HEADERS
  ${CCUTILS_UNICODE_HEADERS}

  LIBS_PUB
  ${CCUTILS_UNICODE_LIBS_PUB}

  INC_DIRS_PUB
  ${CCUTILS_UNICODE_INC_DIRS_PUB}
)

if (CCUTILS_UNICODE_BACKEND STREQUAL icu)
  target_compile_options(ccutils_unicode PUBLIC "-DCCUTILS_UNICODE_BACKEND=icu")
elseif (CCUTILS_UNICODE_BACKEND STREQUAL ccutils)
  target_compile_options(ccutils_unicode PUBLIC "-DCCUTILS_UNICODE_BACKEND=ccutils")
endif()

create_ccutils_component(
  TARGETNAME ccutils_repr
  SUBDIR repr

  SOURCES
  repr

  HEADERS
  repr

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
)

create_ccutils_component(
  TARGETNAME ccutils_logger
  SUBDIR logger

  SOURCES
  logger

  HEADERS
  logger

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
)


create_ccutils_component(
  TARGETNAME ccutils_regexp
  SUBDIR regexp

  SOURCES
  regexp
  regexp_fds_and_enums

  HEADERS
  regexp
  regexp_fds_and_enums

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
  ccutils_repr
)



create_ccutils_component(
  TARGETNAME ccutils_test_suite_tool
  SUBDIR test_suite_tool

  SOURCES
  tst

  HEADERS
  tst

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
  ccutils_logger
)




create_ccutils_component(
  TARGETNAME ccutils_worker01
  SUBDIR worker01

  SOURCES
  Worker01

  HEADERS
  Worker01
)

create_ccutils_component(
  TARGETNAME ccutils_posix_tools
  SUBDIR posix_tools

  SOURCES
  FDCtl
  FDAddress

  HEADERS
  FDCtl
  FDAddress

  LIBS_PUB
  ccutils_unicode
  ccutils_ip

  INC_DIRS_PUB
  ${ccutils_unicode_INCLUDE_DIRS}
)

create_ccutils_component(
  TARGETNAME ccutils_ip
  SUBDIR ip

  SOURCES
  ip

  HEADERS
  ip

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
  ccutils_regexp
)

create_ccutils_component(
  TARGETNAME ccutils_akigo
  SUBDIR akigo

  SOURCES
  builtin
  io
  net
  time
  context
  os

  HEADERS
  _pkg
  builtin
  io
  net
  time
  context
  os

  LIBS_PUB
  ccutils_posix_tools
  ccutils_unicode
  ccutils_errors

  INC_DIRS_PUB
  ICU::uc
  ICU::i18n
  ${ccutils_posix_tools_INCLUDE_DIRS}
  ${ccutils_unicode_INCLUDE_DIRS}
)
