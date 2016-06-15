cmake_minimum_required(VERSION 3.0)
project(BaseProject VERSION 0.0.0.0 LANGUAGES C CXX)

set(USE_STATIC_CRT_LIBRARY OFF CACHE BOOL
    "Use static version of the C run-time library.")

if(MSVC)
    if(USE_STATIC_CRT_LIBRARY)
        foreach(CompilerFlags
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )
            if(${CompilerFlags} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT"
                    ${CompilerFlags} "${${CompilerFlags}}")
            endif(${CompilerFlags} MATCHES "/MD")
        endforeach(CompilerFlags)
    endif(USE_STATIC_CRT_LIBRARY)
endif(MSVC)

add_subdirectory(common)
add_subdirectory(cli)
add_subdirectory(gui)

include_directories(
    thirdparty/include)

if(UNIX OR MINGW)
    set(PLATFORM_LIBRARIES ${CMAKE_DL_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    list(APPEND PLATFORM_LIBRARIES m pthread)
endif(UNIX OR MINGW)

add_executable(cli_app
    ${COMMON_HEADER_FILES}
    ${COMMON_SOURCE_FILES}
    ${CLI_SOURCE_FILES}
    )

target_link_libraries(cli_app
    ${PLATFORM_LIBRARIES}
    )

if(MSVC)
    add_executable(Win32App WIN32
        ${GUICOMMON_SOURCE_FILES}
        ${WIN32COMMON_HEADER_FILES}
        ${WIN32COMMON_SOURCE_FILES}
        ${WIN32COMMON_RESOURCE_FILES}
        ${WIN32APP_HEADER_FILES}
        ${WIN32APP_SOURCE_FILES}
        ${WIN32APP_RESOURCE_FILES}
        )
    target_compile_definitions(Win32App
        PUBLIC -D_UNICODE
        PUBLIC -DUNICODE
        )
endif(MSVC)
