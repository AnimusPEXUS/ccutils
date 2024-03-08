cmake_minimum_required(VERSION 3.27)

project(ccutils_worker01_test01)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_io_tools_test01
	t.cpp
    )

target_link_libraries(
    ccutils_io_tools_test01
    PUBLIC
    ccutils_io_tools
    )

target_include_directories(
    ccutils_io_tools_test01 
    PRIVATE 
    ${ccutils_io_tools_INCLUDE_DIRS}
    )

