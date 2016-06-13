cmake_minimum_required(VERSION 3.0)
project(BaseProject VERSION 0.0.0.0 LANGUAGES C CXX)

add_subdirectory(common)
add_subdirectory(cli)

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
