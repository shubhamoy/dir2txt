cmake_minimum_required(VERSION 3.16)
project(dir2txt LANGUAGES CXX)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source and include directories
include_directories(include)

# Gather all source files
set(SOURCES
    src/main.cpp
    src/Config.cpp
    src/FileFilter.cpp
    src/DirectoryWalker.cpp
    src/FileWriterText.cpp
    src/FileWriterJson.cpp
    src/Utils.cpp
    src/UniversalIgnoreParser.cpp
    src/CommentStripper.cpp
)

# Create the executable
add_executable(dir2txt ${SOURCES})