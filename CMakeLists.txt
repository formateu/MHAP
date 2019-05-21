cmake_minimum_required(VERSION 3.10)
project(mhap_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -Wall -Wextra --coverage")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

#Boost
find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)

add_library(MHAP SHARED
#        src/MurmurHash3.cpp
        src/SequenceStreamer.cpp
        src/HashUtils.cpp
        src/MinHashSketch.cpp
        src/MatchData.cpp
        src/BottomOverlapSketch.cpp
        src/SequenceSketchStreamer.cpp
        src/MinHashSearch.cpp
        src/SequenceSketch.cpp
        src/MatchResult.cpp
        )

target_include_directories(MHAP PRIVATE include)
target_include_directories(MHAP PRIVATE GM3cpp)

add_subdirectory("GM3cpp")

add_executable(mhap
        src/main.cpp
        )

target_include_directories(mhap PRIVATE include)


add_executable(test_mhap
        EXCLUDE_FROM_ALL
        tests/test_main.cpp
        tests/test_SequenceId.cpp
        tests/test_Sequence.cpp
        tests/test_SequenceStreamer.cpp
        tests/test_HashUtils.cpp
        tests/test_HitCounter.cpp
        tests/test_EdgeData.cpp
        tests/test_MatchData.cpp
        tests/test_MinHashSketch.cpp
        tests/test_BottomOverlapSketch.cpp
        tests/test_SequenceSketchStreamer.cpp
        )

target_include_directories(test_mhap PRIVATE include)

target_link_libraries(MHAP
        GM3cpp
        )

target_link_libraries(test_mhap
        ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY}
        MHAP
        )

target_link_libraries(mhap
        MHAP
        ${Boost_LIBRARIES}
        )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/t1.fa ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
