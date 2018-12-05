cmake_minimum_required(VERSION 3.12)
project(mhap_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

#Boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_library(mhap_cpp_lib
        src/MurmurHash3.cpp
        src/SequenceStreamer.cpp
        src/HashUtils.cpp
        src/MinHashSketch.cpp
        src/MatchData.cpp
        src/BottomOverlapSketch.cpp
        src/SequenceSketchStreamer.cpp
        )

add_executable(mhap_cpp
        src/main.cpp
        )

add_executable(test_mhap_cpp
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
        )

target_link_libraries(test_mhap_cpp
        ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY}
        mhap_cpp_lib
        )

target_link_libraries(mhap_cpp
        mhap_cpp_lib
        )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/t1.fa ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)