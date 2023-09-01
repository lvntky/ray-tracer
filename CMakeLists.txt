cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_C_STANDARD 11) # For C projects
# OR


# Add your source files here
set(SOURCE_FILES
    src/main.c
    src/sphere.c 
)

# Add your header files here
set(HEADER_FILES
    include/sphere.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Enable compiler warnings
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Link against the math library (-lm)
target_link_libraries(${PROJECT_NAME} PRIVATE m)

