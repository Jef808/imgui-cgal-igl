if(TARGET igl::data_path)
    return()
endif()

message(STATUS "Third-party: creating target 'igl::data_path'")

include(FetchContent)
FetchContent_Declare(
    libigl_data_path
    GIT_REPOSITORY https://github.com/libigl/libigl-tutorial-data
    GIT_TAG        c1f9ede366d02e3531ecbaec5e3769312f31cccd
)
FetchContent_MakeAvailable(libigl_data_path)

add_library(igl_data_path INTERFACE)
add_library(igl::data_path ALIAS igl_data_path)

target_compile_definitions(igl_data_path INTERFACE "-DLOCAL_IGL_DATA_PATH=\"${libigl_data_path_SOURCE_DIR}\"")

file(CREATE_LINK
  "${libigl_data_path_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/python/data"
  SYMBOLIC
)
 
