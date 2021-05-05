aux_source_directory(. VMM_ARCH_SRCS)
add_library(arch ${VMM_ARCH_SRCS})
target_link_libraries(arch platform)