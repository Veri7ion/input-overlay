set(COMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/common" CACHE STRING "" FORCE)
set(JSON_11_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/json11" CACHE STRING "" FORCE)
set(JSON_11_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/json11/json11.cpp" CACHE STRING "" FORCE)
set(MONGOOSE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/mongoose/mongoose.c" CACHE STRING "" FORCE)

set(MONGOOSE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mongoose" CACHE STRING "" FORCE)

mark_as_advanced(COMMON_HEADERS)
mark_as_advanced(JSON_11_HEADER)
mark_as_advanced(JSON_11_SOURCE)

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/FetchSDL3.cmake")

add_subdirectory(libuiohook EXCLUDE_FROM_ALL)
# The OBS build template sets hidden symbol visibility, but libuiohook has no export macros on Linux/macOS and
# relies on default visibility (hook_run etc. would be hidden and the plugin fails to load).
if(UNIX)
  foreach(_uiohook_target uiohook uiohook-x11 uiohook-wayland uiohook-xrecord)
    if(TARGET ${_uiohook_target})
      set_target_properties(${_uiohook_target} PROPERTIES C_VISIBILITY_PRESET default VISIBILITY_INLINES_HIDDEN OFF)
    endif()
  endforeach()
endif()
if(APPLE)
  target_compile_options(uiohook PRIVATE -Wno-error=return-type)
endif()
