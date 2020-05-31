aux_source_directory(. VMM_SRCS)
aux_source_directory(dev VMM_DEV_SRCS)
add_executable(vmm ${VMM_SRCS} ${VMM_DEV_SRCS})

include_directories(.)

file(COPY fake_bios/bin/fake_bios.bin DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY bios.bin DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ucore.img DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY sfs.img DESTINATION ${EXECUTABLE_OUTPUT_PATH})
