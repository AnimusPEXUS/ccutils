cmake_minimum_required(VERSION 3.27)

project(ccutils_json_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_json_test
	t.cpp
    )

target_link_libraries(
    ccutils_json_test
    PUBLIC
    ccutils_json
    ccutils_logger
    ccutils_test_suite_tool
    )

target_include_directories(
    ccutils_json_test
    PRIVATE 
    # ${ccutils_json_INCLUDE_DIRS}
    # ${ccutils_logger_INCLUDE_DIRS}
    # ${ccutils_test_suite_tool_INCLUDE_DIRS}
    )

