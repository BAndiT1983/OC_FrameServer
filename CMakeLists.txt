cmake_minimum_required (VERSION 3.5)	# Fits for now, update to 3.10 later
project (OC_FrameServer)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror -D_FILE_OFFSET_BITS=64")

file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/DataProvider/*.cpp")

message(STATUS "Sources: " ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} Main.cpp FUSEHandler.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} fuse)
