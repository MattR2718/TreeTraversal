# CMakeList.txt : CMake project for TreeTraversal, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


# find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

find_package(imgui CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
find_package(ImGui-SFML CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (TreeTraversal "TreeTraversal.cpp" "TreeTraversal.h" "algorithms.cpp" "algorithms.h")

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
)

# target_link_libraries (${PROJECT_NAME} sfml-graphics sfml-window sfml-system)


# TODO: Add tests and install targets if needed.
