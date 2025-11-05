cmake_minimum_required(VERSION 3.27)

project(ccutils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("./ccucts/_import_me.cmake")

find_package(PkgConfig REQUIRED)

# set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_DEBUG_LIBS        OFF)
#set(Boost_USE_RELEASE_LIBS       ON)
#set(Boost_USE_MULTITHREADED      ON)

#find_package(Boost REQUIRED COMPONENTS json regex)

include("./cmake_scripts/ccutils_unicode_backend_selector.cmake")

message(STATUS "defining ccutils packages:")

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_utils_cf
  SUBDIR utils

  ITEMS
  cf

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
)

# note: same dir as utils
ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_utils_concepts
  SUBDIR utils

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  concepts

  LIBS_PUB
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_errors
  SUBDIR errors

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  e

  LIBS_PUB
  ccutils_unicode
)

set(CCUTILS_UNICODE_LIBS_PUB_ADD)

if (CCUTILS_UNICODE_BACKEND STREQUAL icu)

  ccucts_define_package(
    DOMAIN_NAME wayround.i2p
    PROJECT_NAME ccutils
    PACKAGE_NAME ccutils_unicode_backend_icu
    SUBDIR unicode_backend_icu

    ADD_COMPILE_FLAGS
    ${ADD_COMPILE_FLAGS}

    ITEMS
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
    CCUTILS_UNICODE_LIBS_PUB_ADD
    ccutils_unicode_backend_icu
  )

endif()

if (CCUTILS_UNICODE_BACKEND STREQUAL ccutils)

  ccucts_define_package(
    DOMAIN_NAME wayround.i2p
    PROJECT_NAME ccutils
    PACKAGE_NAME ccutils_unicode_backend_ccutils
    SUBDIR unicode_backend_ccutils

    ADD_COMPILE_FLAGS
    ${ADD_COMPILE_FLAGS}

    ITEMS
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
    CCUTILS_UNICODE_LIBS_PUB_ADD
    ccutils_unicode_backend_ccutils
  )

endif()

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_unicode
  SUBDIR unicode

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  u

  LIBS_PUB
  ccutils_errors
  ccutils_regexp
  ccutils_repr
  ccutils_utils_cf
  ${CCUTILS_UNICODE_LIBS_PUB_ADD}

  INC_DIRS_PUB
)

if (CCUTILS_UNICODE_BACKEND STREQUAL icu)
  target_compile_options(ccutils_unicode PUBLIC "-DCCUTILS_UNICODE_BACKEND=icu")
elseif (CCUTILS_UNICODE_BACKEND STREQUAL ccutils)
  target_compile_options(ccutils_unicode PUBLIC "-DCCUTILS_UNICODE_BACKEND=ccutils")
endif()

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_repr
  SUBDIR repr

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  repr

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_logger
  SUBDIR logger

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  logger

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_regexp
  SUBDIR regexp


  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  regexp

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
  ccutils_repr
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_test_suite_tool
  SUBDIR test_suite_tool

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  tst

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
  ccutils_logger
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_worker01
  SUBDIR worker01

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  Worker01
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_posix_tools
  SUBDIR posix_tools

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  FDCtl
  FDAddress

  LIBS_PUB
  ccutils_unicode
  ccutils_ip

  #INC_DIRS_PUB
  #  ${ccutils_unicode_INCLUDE_DIRS}
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_ip
  SUBDIR ip

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  ip

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
  ccutils_regexp
)

option(
  CCUTILS_SIGNAL_ENABLE_SIGC_WRAPPER
  "Enable SigC wrapper (compatability layer) for ccutils/signal"
  FALSE
)

set(CCUTILS_SIGNAL_ADD_ITEMS)
if (CCUTILS_SIGNAL_ENABLE_SIGC_WRAPPER)
  list(APPEND CCUTILS_SIGNAL_ADD_ITEMS "signal_sigc_compat")

  pkg_check_modules(sigc REQUIRED IMPORTED_TARGET sigc++-3.0)

  set(CCUTILS_SIGNAL_ADD_COMPILE_FLAGS "${sigc_CFLAGS}")
  set(CCUTILS_SIGNAL_ADD_LINK_FLAGS "${sigc_LDFLAGS}")
endif()

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_signal
  SUBDIR 
  signal
  ${CCUTILS_SIGNAL_ADD_ITEMS}

  ADD_COMPILE_FLAGS
  ${CCUTILS_SIGNAL_ADD_COMPILE_FLAGS}

  ADD_LINK_FLAGS
  ${CCUTILS_SIGNAL_ADD_LINK_FLAGS}	

  ITEMS
  signal
  ${ccutils_signal_ADD_ITEMS}
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_json
  SUBDIR json

  ADD_COMPILE_FLAGS
  ${ADD_COMPILE_FLAGS}

  ITEMS
  json

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
  ccutils_regexp
)

ccucts_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_akigo
  SUBDIR akigo

  ITEMS
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
