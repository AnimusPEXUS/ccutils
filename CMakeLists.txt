cmake_minimum_required(VERSION 3.27)

project(ccutils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(PkgConfig REQUIRED)

# set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_DEBUG_LIBS        OFF)
#set(Boost_USE_RELEASE_LIBS       ON)
#set(Boost_USE_MULTITHREADED      ON)


#find_package(Boost REQUIRED COMPONENTS json regex)

#pkg_check_modules(gtk4 REQUIRED IMPORTED_TARGET gtk4)
#pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)
#pkg_check_modules(gtkmm4 REQUIRED IMPORTED_TARGET gtkmm-4.0)
#pkg_check_modules(sigc REQUIRED IMPORTED_TARGET sigc++-3.0)

#find_package(ICU 73.0 COMPONENTS uc i18n REQUIRED)

#set(ICU_DEBUG ON)

set(CCUTILS_COMPONENTS ccutils_worker_01)

function(create_ccutils_component)
  set(options )
  set(oneValueArgs TARGETNAME SUBDIR)
  set(multiValueArgs SOURCES HEADERS)
  cmake_parse_arguments(
    create_ccutils_component
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  message("  component: ${create_ccutils_component_TARGETNAME}")

  add_library(
    ${create_ccutils_component_TARGETNAME}
  )

  set(formatted_sources_list)
  foreach(i ${create_ccutils_component_SOURCES})
    list(
      APPEND
      formatted_sources_list
      "${create_ccutils_component_SUBDIR}/${i}.cpp"
    )

    foreach(sysname LINUX UNIX WIN32)

      if ((DEFINED ${sysname}) AND (${sysname} EQUAL 1))
	set(nfn "${CMAKE_CURRENT_SOURCE_DIR}/${create_ccutils_component_SUBDIR}/${i}_${sysname}.cpp")
	if (EXISTS ${nfn})
	  list(
	    APPEND
	    formatted_sources_list
	    "${nfn}"
	  )
	endif()
      endif()

    endforeach()

  endforeach()

  target_sources(
    ${create_ccutils_component_TARGETNAME}
    PRIVATE
    ${formatted_sources_list}
  )

  set(formatted_headers_list)
  foreach(i ${create_ccutils_component_HEADERS})
    list(
      APPEND
      formatted_headers_list
      "${create_ccutils_component_SUBDIR}/includes/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}/${i}.hpp"
    )
  endforeach()


  target_sources(
    ${create_ccutils_component_TARGETNAME}
    PUBLIC
    FILE_SET ${create_ccutils_component_TARGETNAME}_headers
    TYPE HEADERS
    BASE_DIRS ${create_ccutils_component_SUBDIR}/includes
    FILES
    ${formatted_headers_list}
  )

  target_include_directories(
    ${create_ccutils_component_TARGETNAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${create_ccutils_component_SUBDIR}/includes>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}>
  )

  install(
    TARGETS ${create_ccutils_component_TARGETNAME}
    FILE_SET ${create_ccutils_component_TARGETNAME}_headers
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wayround_i2p/ccutils/${create_ccutils_component_SUBDIR}
  )


endfunction()



create_ccutils_component(
  TARGETNAME ccutils_worker01
  SUBDIR worker01
  SOURCES
  Worker01
  HEADERS
  Worker01
)

create_ccutils_component(
  TARGETNAME ccutils_io_tools
  SUBDIR io_tools
  SOURCES
  utils
  HEADERS
  utils
)

create_ccutils_component(
  TARGETNAME ccutils_akigo
  SUBDIR akigo
  SOURCES
  builtin
  io
  net
  time
  context
  HEADERS
  _pkg
  builtin
  io
  net
  time
  context
)
