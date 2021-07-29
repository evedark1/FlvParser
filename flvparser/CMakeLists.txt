file(GLOB flvparser_header ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} flvparser_src)
add_library(flvparser ${flvparser_src})

install(TARGETS flvparser ARCHIVE DESTINATION lib)
install(FILES ${flvparser_header} DESTINATION include/flvparser)