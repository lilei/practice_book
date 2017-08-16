AUX_SOURCE_DIRECTORY(./src src_list)
add_executable(practice ${src_list})
TARGET_LINK_LIBRARIES(practice gtest pthread)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)