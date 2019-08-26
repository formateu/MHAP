# MHAP
[![Build Status](https://travis-ci.org/formateu/MHAP.svg?branch=develop)](https://travis-ci.org/formateu/MHAP)

This repository contains simplified, cleaned-up and rewritten in modern C++ version of https://github.com/marbl/MHAP:

MinHash alignment process (MHAP pronounced MAP): locality sensitive hashing to detect overlaps and utilities.

## Build

You must have a [CMake](https://cmake.org/ "CMake") supporting C++17 and [Boost Libraries](http://boost.org/ "Boost") installed on your system. To checkout and build run:

    git clone https://github.com/formateu/mhap-cpp
    cd mhap-cpp
    mkdir build
    cd build
    cmake ..
    make

## Cite
 - Berlin K, Koren S, Chin CS, Drake PJ, Landolin JM, Phillippy AM [Assembling Large Genomes with Single-Molecule Sequencing and Locality Sensitive Hashing](http://www.nature.com/nbt/journal/v33/n6/abs/nbt.3238.html "nb"). Nature Biotechnology. (2015).

## License

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
