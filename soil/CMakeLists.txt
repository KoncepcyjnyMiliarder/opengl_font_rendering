cmake_minimum_required(VERSION 3.0)

project(soil)

add_library(${PROJECT_NAME} STATIC image_DXT.c image_helper.c SOIL.c stb_image_aug.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})