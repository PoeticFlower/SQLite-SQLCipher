file(GLOB COMMON_HEADER_FILES_INTERNAL
    LIST_DIRECTORIES false
    common.h
    opencl_wrapper.h
    sqlite_wrapper.h
    threading_wrapper.h
    thread_pool.h
    )
file(GLOB COMMON_SOURCE_FILES_INTERNAL
    LIST_DIRECTORIES false
    common.cpp
    opencl_wrapper.cpp
    sqlite_wrapper.cpp
    threading_wrapper.cpp
    thread_pool.cpp
    )

set(COMMON_HEADER_FILES
    ${COMMON_HEADER_FILES_INTERNAL}
    PARENT_SCOPE
    )
set(COMMON_SOURCE_FILES
    ${COMMON_SOURCE_FILES_INTERNAL}
    PARENT_SCOPE
    )
