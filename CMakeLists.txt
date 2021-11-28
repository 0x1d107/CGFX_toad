# CMakeList.txt : CMake project for cgfx_lab2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)


project ("cgfx_lab2")
add_compile_options(-fsanitize=address -g)
add_link_options(-fsanitize=address )

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
)
set(SFML_STATIC_LIBRARIES TRUE)
FetchContent_MakeAvailable( sfml )

# Add source to this project's executable.
add_executable (cgfx_lab2 "cgfx_lab2.cpp" "cgfx_lab2.h")

target_link_libraries(cgfx_lab2 sfml-graphics sfml-system sfml-audio)


# TODO: Add tests and install targets if needed.
