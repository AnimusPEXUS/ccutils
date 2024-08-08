cmake_minimum_required(VERSION 3.27)

project(ccutils_ip_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_ip_test
	t.cpp
    )

target_link_libraries(
    ccutils_ip_test
    PUBLIC
    ccutils_ip
    ccutils_logger
    ccutils_test_suite_tool
    )

target_include_directories(
    ccutils_ip_test
    PRIVATE 
    ${ccutils_akigo_INCLUDE_DIRS}
    )

