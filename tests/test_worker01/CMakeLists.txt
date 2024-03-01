cmake_minimum_required(VERSION 3.27)

project(ccutils_worker01_test01)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../.. ccutils-bindir)

add_executable(
	ccutils_worker01_test01
	t.cpp
    )

target_link_libraries(
    ccutils_worker01_test01
    PUBLIC
    ccutils_worker01
    )

target_include_directories(
    ccutils_worker01_test01 
    PRIVATE 
    ${ccutils_worker01_INCLUDE_DIRS}
    )

