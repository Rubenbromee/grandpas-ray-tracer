set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(grandpas_ray_tracer)

file(GLOB SOURCES "raytracer/*.cpp")
file(GLOB HEADERS "raytracer/*.h")

add_executable(grandpas_ray_tracer_exe ${SOURCES} ${HEADERS})

set(glm_dir $ENV{GLM_DIR})

file(TO_CMAKE_PATH "${glm_dir}" glm_dir_normalized)

target_include_directories(grandpas_ray_tracer_exe PRIVATE ${glm_dir_normalized})