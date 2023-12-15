add_library(day1 SHARED main.hxx main.cxx)

find_package(range-v3)

add_executable(trebuchet main.cxx)
target_link_libraries(day1 range-v3::range-v3)
target_link_libraries(trebuchet day1)

install(FILES input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
