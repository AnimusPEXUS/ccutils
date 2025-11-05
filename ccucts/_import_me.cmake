
# ccutils cmake toolset (CCUCTS for short, yes B) )
#
#  this toolset (which you couldd use in your projects too),
#  is for my ccutils and other my c/c++ projects, using cmake to build projects.
#
#  I decided to make CCUCTS becouse I didn't wanted to write same cmake scripts individually
#  to all my projects, so I decided to make one separate project with all my common
#  cmake scripts and import them everywhere I need.
#
#  currently CCUCTS is just a subdir of ccutils. 
#  when it's become more stable, I will separate it to it's own repository

if(DEFINED CCUCTS_VERSION)
  return()
endif()

set(CCUCTS_VERSION 0.1)

# all other scripts in this dir should message(FATAL_ERROR) 
# if CCUCTS_DEFINITION_PROCESS is undefined

set(CCUCTS_DEFINITION_PROCESS TRUE)

cmake_path(GET CMAKE_CURRENT_LIST_FILE PARENT_PATH CCUCTS_ROOT_DIR)

# include modules here

include("${CCUCTS_ROOT_DIR}/ccucts_detect_cpu_os.cmake")
include("${CCUCTS_ROOT_DIR}/ccucts_define_package.cmake")

# do CCUCTS internal initialization

ccucts_detect_cpu_os(CCUCTS_CALCULATED_HOST_CPU CCUCTS_CALCULATED_HOST_OS)

message(STATUS "using CCUCTS (ver ${CCUCTS_VERSION})")
message(STATUS "  CCUCTS configuring results:")
message(STATUS "     detected host CPU: ${CCUCTS_CALCULATED_HOST_CPU}")
message(STATUS "     detected host OS : ${CCUCTS_CALCULATED_HOST_OS}")
message(STATUS "     CCUCTS root dir  : ${CCUCTS_ROOT_DIR}")

block()
  set(t_l 0)
  string(LENGTH "${CCUCTS_CALCULATED_HOST_CPU}" t_l)
  if (t_l EQUAL 0)
    message(FATAL_ERROR "error detecting CPU")
  endif()

  set(t_l 0)
  string(LENGTH "${CCUCTS_CALCULATED_HOST_OS}" t_l)
  if (t_l EQUAL 0)
    message(FATAL_ERROR "error detecting OS")
  endif()
endblock()

unset(CCUCTS_DEFINITION_PROCESS)
