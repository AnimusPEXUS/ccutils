
set(
  CCUTILS_UNICODE_BACKEND
  icu
  CACHE
  STRING
  "set to 'icu' or 'ccutils' - select backend used by ccutils_unicode library"
)

block(SCOPE_FOR VARIABLES)
  set(CCUTILS_UNICODE_BACKEND_correct OFF)
  foreach(i IN ITEMS icu ccutils)
    if ("${i}" STREQUAL "${CCUTILS_UNICODE_BACKEND}")
      set(CCUTILS_UNICODE_BACKEND_correct ON)
      break()
    endif()
  endforeach()
  if (NOT CCUTILS_UNICODE_BACKEND_correct)
    message(FATAL_ERROR "invalid value for CCUTILS_UNICODE_BACKEND")
  endif()
endblock()

message(STATUS "CCUTILS_UNICODE_BACKEND is ${CCUTILS_UNICODE_BACKEND}")

if (CCUTILS_UNICODE_BACKEND STREQUAL icu)

  #set(ICU_DEBUG ON)

  find_package(ICU 73.0 COMPONENTS uc i18n io REQUIRED)

endif()

