cmake_minimum_required(VERSION 3.16...3.21)
project(io_client VERSION 5.0.0)

string(TIMESTAMP TODAY "%Y.%m.%d %H:%M")
add_compile_definitions(TIMESTAMP="${TODAY}")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    add_definitions(-DUNIX=1)
    add_definitions(-DLINUX=1)
    set(client_PLATFORM_DEPS
            pthread)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/FetchSDL3.cmake")

if (WIN32 OR WIN64)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(client_PLATFORM_DEPS ws2_32 iphlpapi)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(client_PLATFORM_DEPS wsock32 iphlpapi)
    endif()
endif()

set(io_client_SOURCES
    ../deps/mongoose/mongoose.c
    ../deps/mongoose/mongoose.h
    ../deps/json11/json11.cpp

    src/argparse.c
    src/argparse.h
    src/io_client.cpp
    src/client_util.cpp
    src/client_util.hpp
    src/network_helper.cpp
    src/network_helper.hpp
    src/gamepad_helper.cpp
    src/gamepad_helper.hpp
    src/uiohook_helper.cpp
    src/uiohook_helper.hpp)

add_executable(${PROJECT_NAME} ${io_client_SOURCES})

# Only the libraries are needed, not the rest of what libuiohook installs
add_subdirectory(../deps/libuiohook ${CMAKE_CURRENT_BINARY_DIR}/libuiohook EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME}
    uiohook
    ${SDL3_LIBRARIES}
    ${client_PLATFORM_DEPS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL3_INCLUDE_DIRS}
    ../deps/libuiohook/include
    ../deps/common
    ../deps/json11
    ../deps/mongoose
)

if (WIN32 OR WIN64)
    if (MSVC)
        add_definitions(/MP /d2FH4-)
    endif()

    # Copy dependencies
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL3_DLLS}
        $<TARGET_FILE:uiohook>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    install(FILES ${SDL3_DLLS} $<TARGET_FILE:uiohook> DESTINATION ${PROJECT_NAME})

elseif (UNIX AND NOT APPLE)
    # libuiohook loads one of its back-ends at runtime from the directory it is in. In the plugin folder
    # the libraries are either next to the client or in bin/64bit for OBS Studio 32 and older
    add_dependencies(${PROJECT_NAME} uiohook-x11 uiohook-wayland uiohook-xrecord)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/bin/64bit")
    install(FILES
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:uiohook>
        $<TARGET_FILE:uiohook-x11>
        $<TARGET_FILE:uiohook-wayland>
        $<TARGET_FILE:uiohook-xrecord>
        DESTINATION ${PROJECT_NAME})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})
