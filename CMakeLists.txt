cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

set(CGAL_DIR /home/jfa/repos/cgal) #/Installation/lib/cmake/CGAL)

find_package(Boost REQUIRED)
find_package(CGAL REQUIRED) # OPTIONAL_COMPONENTS Qt5

# add_library(cgal_data_dir INTERFACE)
# target_compile_definitions(cgal_data_dir INTERFACE
#   "-DCGAL_DATA_DIR=\"${CGAL_DIR}/Data/data\"")

# set(BUILD_SHARED_LIBS OFF)
# set(BUILD_EXAMPLES OFF)
# set(LIBIGL_IMGUI ON)

# include(libigl)

add_subdirectory(Viewer)

# target_link_libraries(viewer PUBLIC
#   cgal_data_dir
#   CGAL::CGAL
#   igl::glfw
#   igl::imgui)

# create_single_source_cgal_program("gradient.cpp")
add_executable(gradient
  gradient.cpp)
target_link_libraries(gradient PUBLIC
  viewer)
