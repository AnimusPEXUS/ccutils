cmake_minimum_required(VERSION 3.27)

project(ccutils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# system detection. this is used to select/provide system-specific sources and headers

include("./cmake_scripts/ccutils_detect_cpu_os.cmake")

set(HOST_CPU)
set(HOST_OS)

ccutils_detect_cpu_os(HOST_CPU HOST_OS)

message(NOTICE "configuring for '${HOST_OS}' on '${HOST_CPU}'")

block()
  set(t_l 0)
  string(LENGTH "${HOST_CPU}" t_l)
  if (t_l EQUAL 0)
    message(FATAL_ERROR "error detecting CPU")
  endif()

  set(t_l 0)
  string(LENGTH "${HOST_OS}" t_l)
  if (t_l EQUAL 0)
    message(FATAL_ERROR "error detecting OS")
  endif()
endblock()


find_package(PkgConfig REQUIRED)

# set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_DEBUG_LIBS        OFF)
#set(Boost_USE_RELEASE_LIBS       ON)
#set(Boost_USE_MULTITHREADED      ON)

#find_package(Boost REQUIRED COMPONENTS json regex)

include("./cmake_scripts/create_ccutils_component.cmake")

include("./cmake_scripts/ccutils_unicode_backend_selector.cmake")
include("./cmake_scripts/ccutils_define_package.cmake")

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_utils_cf
  SUBDIR utils
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  cf

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
)

# note: same dir as utils
ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_utils_concepts
  SUBDIR utils
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  concepts

  LIBS_PUB
)

set(CCUTILS_UNICODE_LIBS_PUB_ADD)

if (CCUTILS_UNICODE_BACKEND STREQUAL icu)

  #set(ICU_DEBUG ON)

  find_package(ICU 73.0 COMPONENTS uc i18n io REQUIRED)

  ccutils_define_package(
    DOMAIN_NAME wayround.i2p
    PROJECT_NAME ccutils
    PACKAGE_NAME ccutils_unicode_backend_icu
    SUBDIR unicode_backend_icu
    CPU ${HOST_CPU}
    OS ${HOST_OS}

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

  ccutils_define_package(
    DOMAIN_NAME wayround.i2p
    PROJECT_NAME ccutils
    PACKAGE_NAME ccutils_unicode_backend_ccutils
    SUBDIR unicode_backend_ccutils
    CPU ${HOST_CPU}
    OS ${HOST_OS}

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

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_unicode
  SUBDIR unicode
  CPU ${HOST_CPU}
  OS ${HOST_OS}

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

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_repr
  SUBDIR repr
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  repr

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_logger
  SUBDIR logger
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  logger

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_regexp
  SUBDIR regexp
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  regexp

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
  ccutils_repr
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_test_suite_tool
  SUBDIR test_suite_tool
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  tst

  LIBS_PUB
  ccutils_errors
  ccutils_unicode
  ccutils_logger
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_worker01
  SUBDIR worker01
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  Worker01
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_posix_tools
  SUBDIR posix_tools
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  FDCtl
  FDAddress

  LIBS_PUB
  ccutils_unicode
  ccutils_ip

  #INC_DIRS_PUB
  #  ${ccutils_unicode_INCLUDE_DIRS}
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_ip
  SUBDIR ip
  CPU ${HOST_CPU}
  OS ${HOST_OS}

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

set(ccutils_signal_ADD_ITEMS)
if (CCUTILS_SIGNAL_ENABLE_SIGC_WRAPPER)
  list(APPEND ccutils_signal_ADD_ITEMS "signal_sigc_compat")
endif()

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_signal
  SUBDIR signal
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  signal
  ${ccutils_signal_ADD_ITEMS}
)


if (CCUTILS_SIGNAL_ENABLE_SIGC_WRAPPER)
  pkg_check_modules(sigc REQUIRED IMPORTED_TARGET sigc++-3.0)

  target_compile_options(ccutils_signal PUBLIC "${sigc_CFLAGS}")
  target_link_options(ccutils_signal PUBLIC "${sigc_LDFLAGS}")
endif()

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_json
  SUBDIR json
  CPU ${HOST_CPU}
  OS ${HOST_OS}

  ITEMS
  json

  LIBS_PUB
  ccutils_unicode
  ccutils_errors
  ccutils_regexp
)

ccutils_define_package(
  DOMAIN_NAME wayround.i2p
  PROJECT_NAME ccutils
  PACKAGE_NAME ccutils_akigo
  SUBDIR akigo
  CPU ${HOST_CPU}
  OS ${HOST_OS}

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
