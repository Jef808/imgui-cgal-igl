cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake /usr/local/lib/cmake)

set(CGAL_DIR /home/jfa/repos/cgal) #/Installation/lib/cmake/CGAL)

find_package(Boost REQUIRED)
find_package(CGAL REQUIRED) # OPTIONAL_COMPONENTS Qt5
find_package(Ceres REQUIRED)

if (NOT Ceres_FOUND)
  message(STATUS "Failed to find Ceres library")
  return()
endif()

# add_library(cgal_data_dir INTERFACE)
# target_compile_definitions(cgal_data_dir INTERFACE
#   "-DCGAL_DATA_DIR=\"${CGAL_DIR}/Data/data\"")

set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLES OFF)
set(LIBIGL_IMGUI ON)

include(libigl)

#add_subdirectory(Viewer)

# target_link_libraries(viewer PUBLIC
#   cgal_data_dir
#   CGAL::CGAL
#   igl::glfw
#   igl::imgui)

create_single_source_cgal_program("example.cpp")
# add_executable(gradient
#   gradient.cpp)
target_link_libraries(example PUBLIC
  igl::glfw
  igl::imgui
  ${CERES_LIBRARIES})
# target_precompile_headers(example PUBLIC
#   "CGAL/Exact_predicates_inexact_constructions_kernel.h"
#   "CGAL/Point_3.h"
#   "CGAL/Surface_mesh.h"
#   "CGAL/boost/graph/named_params_helper.h"
#   "CGAL/boost/graph/properties.h"
#   "CGAL/property_map.h"
#   "CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h")
