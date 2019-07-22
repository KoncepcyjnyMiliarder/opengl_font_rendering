cmake_minimum_required(VERSION 3.0)

project(gujec)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(soil)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(SOURCES
  src/generic_shader.cpp
  src/font.cpp
  src/font_loader.cpp
  src/font_renderer.cpp
  src/texture.cpp
  src/cached_text.cpp
)

set(INCLUDES
  inc/generic_shader.hpp
  inc/binary_deserializer.hpp
  inc/font.hpp
  inc/font_loader.hpp
  inc/font_renderer.hpp
  inc/texture.hpp
  inc/cached_text.hpp  
)

set(SHADERS
  rsrc/color.fs
  rsrc/color.vs
  rsrc/text.fs
  rsrc/text.vs
  rsrc/screen.fs
  rsrc/screen.vs
  rsrc/blur.fs
  rsrc/blur.vs
)

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${INCLUDES} ${SHADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
  inc
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  soil
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glm
  ${OPENGL_gl_LIBRARY}
  ${GLEW_LIBRARIES}
  soil
)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/rsrc
  ${CMAKE_CURRENT_BINARY_DIR}/rsrc
)
