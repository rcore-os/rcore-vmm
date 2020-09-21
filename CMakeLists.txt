cmake_minimum_required(VERSION 3.0)

project(rcore_vmm)
set(CMAKE_OSX_DEPLOYMENT_TARGET "")

# Path
aux_source_directory(src SRCS)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# Toolchain
set(PREFIX ${ARCH}-linux-musl-)

if (${ARCH} STREQUAL x86_64)
    set(CMAKE_C_FLAGS       "-m64")
else()
    message("Unsupported arch: ${ARCH}")
endif ()
set(CMAKE_ASM_COMPILER      ${PREFIX}gcc)
set(CMAKE_C_COMPILER        ${PREFIX}gcc)
set(CMAKE_RANLIB            ${PREFIX}ranlib)
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -ggdb -Wall")
set(CMAKE_ASM_FLAGS         ${CMAKE_C_FLAGS})
set(CMAKE_C_LINK_FLAGS      "${LINK_FLAGS}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

add_subdirectory(src)
