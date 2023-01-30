project(Zerializer)

add_library(zerializer zerializer.cpp)
# add_library(Zerializer SHARED zerializer.cpp)

target_include_directories(zerializer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
