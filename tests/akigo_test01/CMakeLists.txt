cmake_minimum_required(VERSION 3.27)

project(ccutils_akigo_test_unix)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_akigo_test_unix
	t_net_unix_server_cc.cpp
    )

target_link_libraries(
    ccutils_akigo_test_unix
    PUBLIC
    ccutils_akigo
    )

target_include_directories(
    ccutils_akigo_test_unix
    PRIVATE 
    ${ccutils_akigo_INCLUDE_DIRS}
    )

