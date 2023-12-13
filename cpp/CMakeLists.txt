cmake_minimum_required(VERSION 3.18)
project(osm_routing)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/includes/nlohmann)


add_executable(
  osm_routing_test
  includes/tinyxml2.cpp
  osm_routing.cpp
  osm_routing_test.cpp 
)



enable_testing()
target_link_libraries(
  osm_routing_test GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(osm_routing_test)

# # Now simply link against gtest or gtest_main as needed. Eg
# add_executable(tinyxml2 includes/tinyxml2.cpp)
# add_executable(osm_routing osm_routing.cpp)
# add_executable(osm_routing_test osm_routing_test.cpp)
# target_link_libraries(tinyxml2 gtest_main)
# target_link_libraries(osm_routing gtest_main)
# target_link_libraries(osm_routing_test gtest_main)
# add_test(NAME osm_routing_test COMMAND osm_routing_test)
