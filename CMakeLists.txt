cmake_minimum_required(VERSION 3.5)
project(kramer LANGUAGES CXX)

option(BUILD_TEST "Build the project test file(s)" ON)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/kramer/variable.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/kramer/variable.hpp"
)
file(GLOB HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/kramer/variable.hpp"
)
add_library(kramer ${SOURCES})

target_include_directories(kramer PUBLIC
"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/kramer>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/>"
)
install(TARGETS kramer
        EXPORT kramerConfig
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        DESTINATION lib)
install(FILES ${HEADERS}
        DESTINATION include)
install(EXPORT kramerConfig
        NAMESPACE kramer::
        DESTINATION lib/cmake/kramer)
# export(TARGETS kramerConfig
#         NAMESPACE kramer::
#         FILE "${PROJECT_BINARY_DIR}/kramerConfig.cmake")

if (BUILD_TEST)
        add_subdirectory(3rd/googletest)
        enable_testing()
        include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
        add_executable(tests exec/tests/main.cpp)
        target_link_libraries(tests gtest gtest_main kramer)
endif()