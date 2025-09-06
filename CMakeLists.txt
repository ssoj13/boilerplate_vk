cmake_minimum_required(VERSION 3.20)

project(VulkanApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vcpkg toolchain file
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Include directories
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Function to compile GLSL to SPIR-V
function(compile_shader SOURCE TARGET)
    add_custom_command(
        OUTPUT ${TARGET}
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} --target-env vulkan1.0 -V ${SOURCE} -o ${TARGET}
        DEPENDS ${SOURCE}
        COMMENT "Compiling ${SOURCE} to ${TARGET}"
    )
endfunction()

# Compile shaders
compile_shader(${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert ${CMAKE_CURRENT_BINARY_DIR}/shader.vert.spv)
compile_shader(${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag ${CMAKE_CURRENT_BINARY_DIR}/shader.frag.spv)

# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/VulkanApp.cpp
    src/VulkanApp.h
    src/Mesh.cpp
    src/Mesh.h
    ${CMAKE_CURRENT_BINARY_DIR}/shader.vert.spv
    ${CMAKE_CURRENT_BINARY_DIR}/shader.frag.spv
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    glfw
    ${Vulkan_LIBRARIES}
)

# Compiler-specific options for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Copy compiled shader files to executable directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/shader.vert.spv $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader.vert.spv
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/shader.frag.spv $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader.frag.spv
)