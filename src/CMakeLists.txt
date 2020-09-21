cmake_minimum_required(VERSION 3.0)

project(rcore_vmm)
set(CMAKE_OSX_DEPLOYMENT_TARGET "")

set(PREFIX ${ARCH}-linux-musl-)

if (${ARCH} STREQUAL x86_64)
    set(CMAKE_C_FLAGS       "-m64")
else()
    message("Unsupported arch: ${ARCH}")
endif()
set(CMAKE_ASM_COMPILER      ${PREFIX}gcc)
set(CMAKE_C_COMPILER        ${PREFIX}gcc)
set(CMAKE_RANLIB            ${PREFIX}ranlib)
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -ggdb -Wall")
set(CMAKE_ASM_FLAGS         ${CMAKE_C_FLAGS})
set(CMAKE_C_LINK_FLAGS      "${LINK_FLAGS}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

aux_source_directory(. VMM_SRCS)
aux_source_directory(dev VMM_DEV_SRCS)
add_executable(vmm ${VMM_SRCS} ${VMM_DEV_SRCS})

include_directories(.)
include_directories(include)
