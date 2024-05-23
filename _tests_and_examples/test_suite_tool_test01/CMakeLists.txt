cmake_minimum_required(VERSION 3.27)

project(ccutils_test_suite_tool_test_suite)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_test_suite_tool_test_suite
	t.cpp
    )

target_link_libraries(
    ccutils_test_suite_tool_test_suite
    PUBLIC
    ccutils_test_suite_tool
    )

target_include_directories(
    ccutils_test_suite_tool_test_suite
    PRIVATE 
    ${ccutils_test_suite_tool_INCLUDE_DIRS}
    )

