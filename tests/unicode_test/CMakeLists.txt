cmake_minimum_required(VERSION 3.27)

project(ccutils_unicode_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_unicode_test
	t.cpp
    )

target_link_libraries(
    ccutils_unicode_test
    PUBLIC
    ccutils_unicode
    )

target_include_directories(
    ccutils_unicode_test
    PRIVATE 
    )

