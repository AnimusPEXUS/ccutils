cmake_minimum_required(VERSION 3.27)

project(ccutils_regexp_test01)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_regexp_test01
	t.cpp
    )

target_link_libraries(
    ccutils_regexp_test01
    PUBLIC
    ccutils_unicode
    ccutils_regexp
    ccutils_test_suite_tool
    )

target_include_directories(
    ccutils_regexp_test01
    PRIVATE 
    )

