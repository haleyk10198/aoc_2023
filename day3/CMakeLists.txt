add_executable(day3 main.cxx main.hxx)

install(FILES input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(day3 range-v3::range-v3)
