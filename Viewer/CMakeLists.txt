set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLES OFF)
set(LIBIGL_IMGUI ON)

find_package(Boost REQUIRED)
find_package(CGAL REQUIRED)
include(libigl)

add_library(cgal_data_dir INTERFACE)
target_compile_definitions(cgal_data_dir INTERFACE
  "-DCGAL_DATA_DIR=\"${CGAL_DIR}/Data/data\"")

add_library(viewer STATIC
  my_file_dialog_open.h
  imgui/CGALPlugin.h
  imgui/MeshMenu.h
  imgui/KeyHandler.h
  imgui/KeyHandler.cpp
  viewer.h
  viewer.cpp)
target_link_libraries(viewer PUBLIC
  cgal_data_dir
  CGAL::CGAL
  igl::glfw
  igl::imgui)
target_include_directories(viewer PUBLIC
  ${CMAKE_SOURCE_DIR})
