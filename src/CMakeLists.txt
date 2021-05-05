cmake_minimum_required(VERSION 3.0)

project(rcore_vmm)
set(CMAKE_OSX_DEPLOYMENT_TARGET "")

if (${ARCH} STREQUAL x86_64)
    set(CMAKE_C_FLAGS       "-m64")
elseif (${ARCH} STREQUAL riscv64)
else()
    message( FATAL_ERROR "Unsupported arch: ${ARCH}")
endif()
set(CMAKE_ASM_COMPILER      ${GCCPREFIX}gcc)
set(CMAKE_C_COMPILER        ${GCCPREFIX}gcc)
set(CMAKE_RANLIB            ${GCCPREFIX}ranlib)
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -ggdb -Wall -static")
set(CMAKE_ASM_FLAGS         ${CMAKE_C_FLAGS})
set(CMAKE_C_LINK_FLAGS      "${LINK_FLAGS}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

if (${ARCH} STREQUAL x86_64)
    include_directories(${PROJECT_SOURCE_DIR}/include/arch/amd64)
elseif (${ARCH} STREQUAL riscv64)
    include_directories(${PROJECT_SOURCE_DIR}/include/arch/riscv64)
else()
    message( FATAL_ERROR "Unsupported arch: ${ARCH}")
endif()
include_directories(.)
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. VMM_SRCS)
add_subdirectory(arch)
add_subdirectory(dev)

# message( "FILES: ${VMM_SRCS} ${VMM_DEV_SRCS} ${VMM_ARCH_SRCS}" )
add_executable(vmm ${VMM_SRCS} ${VMM_DEV_SRCS} ${VMM_ARCH_SRCS})


target_link_libraries(vmm platform arch)

