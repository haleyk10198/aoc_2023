add_executable(day2 main.cxx)
add_executable(day2p2 part2.cxx)

target_link_libraries(day2p2 range-v3::range-v3)
install(FILES input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

