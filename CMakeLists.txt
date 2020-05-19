aux_source_directory(. VMM_SRCS)
add_executable(vmm ${VMM_SRCS})

file(COPY bios.bin DESTINATION ${EXECUTABLE_OUTPUT_PATH})
