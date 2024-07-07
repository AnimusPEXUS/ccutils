cmake_minimum_required(VERSION 3.27)

project("wayround_i2p_ccutils_ucd.all.flat.xml_codegen")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
include(FetchContent)
# include(GNUInstallDirs)


FetchContent_Declare(
  libxmlpp
  GIT_REPOSITORY "https://github.com/leethomason/tinyxml2.git"
  GIT_TAG        "v10.0.0"
  GIT_PROGRESS   TRUE
  #GIT_SHALLOW    TRUE
)


FetchContent_MakeAvailable(libxmlpp)

add_subdirectory(.. ccutils-bindir)

add_executable(
	"wayround_i2p_ccutils_ucd.all.flat.xml_codegen"
	main.cpp
    )

target_link_libraries(
    "wayround_i2p_ccutils_ucd.all.flat.xml_codegen"
    PUBLIC
    tinyxml2
    )

#target_include_directories(
#    ccutils_akigo_test_unix
#    PRIVATE 
#    ${ccutils_akigo_INCLUDE_DIRS}
#    )

