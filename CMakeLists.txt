cmake_minimum_required(VERSION 3.10)

#Project Name
project(File_Transfer VERSION 0.6
					  DESCRIPTION "Advent of Code 2021."
					  LANGUAGES CXX)

cmake_policy(SET CMP0079 NEW)

#Directories
include_directories (./1)

#Libraries
#target_link_libraries (NFT PUBLIC FM Client Server)

#Executables
add_executable (main main.cpp)

#Compile Options
target_compile_features (main PUBLIC cxx_std_17)

if(MSVC)
	add_compile_options(/O2)
else()
	add_compile_options(-O2)
endif()


