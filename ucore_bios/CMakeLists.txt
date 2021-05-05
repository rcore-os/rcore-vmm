cmake_minimum_required(VERSION 3.0)

project(ucore_bios)
set(CMAKE_OSX_DEPLOYMENT_TARGET "")
enable_language(ASM)

set(PREFIX ${ARCH}-unknown-linux-musl-)
if (${ARCH} STREQUAL x86_64)
    set(CMAKE_C_FLAGS       "-m16")
else()
    message("Unsupported arch: ${ARCH}")
endif()
set(CMAKE_ASM_COMPILER      ${PREFIX}gcc)
set(CMAKE_C_COMPILER        ${PREFIX}gcc)
set(CMAKE_RANLIB            ${PREFIX}ranlib)
set(CMAKE_OBJCOPY           ${PREFIX}objcopy)
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -ggdb -Wall")
set(CMAKE_ASM_FLAGS         ${CMAKE_C_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_LINK_FLAGS} -nostdlib -N -T ${CMAKE_CURRENT_SOURCE_DIR}/bios.ld")

set(BIOS_ASM bios.S setup_ivt.S)
add_executable(bios ${BIOS_ASM})
set_target_properties(bios PROPERTIES SUFFIX ".elf")

add_custom_command(
    TARGET bios
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary $<TARGET_FILE:bios> ${PROJECT_NAME}.bin
)
