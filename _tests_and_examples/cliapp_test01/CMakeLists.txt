cmake_minimum_required(VERSION 3.27)

project(ccutils_cliapp_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_cliapp_test
	t.cpp
    )

target_link_libraries(
    ccutils_cliapp_test
    PUBLIC
    ccutils_cliapp
    ccutils_logger
    ccutils_unicode
    ccutils_test_suite_tool
    )

target_include_directories(
    ccutils_cliapp_test
    PRIVATE 
    )

