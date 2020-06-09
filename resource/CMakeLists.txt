# just copies resources to binary directory

file(COPY cube DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY teapot DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})