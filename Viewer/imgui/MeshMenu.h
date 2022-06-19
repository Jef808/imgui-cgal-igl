#ifndef MESHMENU_H_
#define MESHMENU_H_

#include "Viewer/my_file_dialog_open.h"
#include "Viewer/imgui/CGALPlugin.h"

#include <CGAL/Surface_mesh/Surface_mesh_fwd.h>

#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiWidget.h>

#include <string>
#include <iostream>

// namespace igl::opengl::glfw {
//   class Viewer;
//   namespace imgui {
//     class ImGuiPlugin;
//   }
// }

namespace ImGuiViewer {


template< typename Point >
class MeshMenuWidget : public igl::opengl::glfw::imgui::ImGuiWidget {
public:
  using Plugin = ImGuiCGALPlugin< Point >;

  MeshMenuWidget();

  /**
   * Store a pointer to the ImGuiCGALPlugin object upon initialisation
   * and set its `load_callback` function so that it prepends the name of
   * the mesh with the correct path.
   */
  void init(igl::opengl::glfw::Viewer *_viewer,
            igl::opengl::glfw::imgui::ImGuiPlugin *_plugin) override;

  /**
   * Here we specify any custom drawing steps
   */
  void draw() override;

  /** Called at the beginning of `draw_data_prompt()` */
  std::function<void(void)> set_style_callback;

private:
  Plugin* cgal_plugin;
  std::string mesh_dir;

  /**
   * The method used to draw the menu items related to loading and saving data.
   */
  void draw_data_prompt();
};



template< typename Point >
MeshMenuWidget< Point >::MeshMenuWidget()
  : ImGuiWidget()
  , mesh_dir{ CGAL::data_file_path("/meshes/") }
{
  name = "mesh_menu";
}

template< typename Point >
void MeshMenuWidget< Point >::init(igl::opengl::glfw::Viewer* _viewer,
                                   igl::opengl::glfw::imgui::ImGuiPlugin* _plugin)
{
    viewer = _viewer;
    plugin = _plugin;

    auto plugin_it = std::find_if(_viewer->plugins.begin(), _viewer->plugins.end(), [](auto* p) {
        return p->plugin_name == "cgal_plugin";
    });

    if (plugin_it == viewer->plugins.end()) {
        std::cerr << "ImGuiCGALPlugin not found while initializing the mesh repo Widget!" << std::endl;
        return;
    }
    cgal_plugin = (Plugin*)*plugin_it;

    const size_t path_sz = mesh_dir.size();
    mesh_dir.resize(path_sz + 32);

    cgal_plugin->load_filename_callback = [&path=mesh_dir, sz=path_sz]
        (const std::string& fn)
    {
        std::copy(fn.begin(), fn.end(), path.begin() + sz);
        return path;
    };
}


template< typename Point >
void MeshMenuWidget< Point >::draw() {
    draw_data_prompt();
}


template< typename Point >
void MeshMenuWidget< Point >::draw_data_prompt() {
  if (set_style_callback) {
    set_style_callback();
  }
  /* To load/save the environment */
  if (ImGui::CollapsingHeader("Workspace", ImGuiTreeNodeFlags_DefaultOpen))
  {
    float w = ImGui::GetContentRegionAvail().x;
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("Load##Workspace", ImVec2((w-p)/2.f, 0)))
    {
      viewer->load_scene();
    }
    ImGui::SameLine(0, p);
    if (ImGui::Button("Save##Workspace", ImVec2((w-p)/2.f, 0)))
    {
      viewer->save_scene();
    }
  }
  /* To choose which mesh we want to open/save */
  if (ImGui::CollapsingHeader("Mesh Data", ImGuiTreeNodeFlags_DefaultOpen)) {
    float w = ImGui::GetContentRegionAvail().x;
    float p = ImGui::GetStyle().FramePadding.x;

    if (ImGui::Button("Load##Mesh", ImVec2((w-p)/2.f, 0)))
    {
      std::string fname = my_file_dialog_open(mesh_dir);
      viewer->load_mesh_from_file(fname);
    }
    ImGui::SameLine(0, p);
    if (ImGui::Button("Save##Mesh", ImVec2((w-p)/2.f, 0)))
    {
      viewer->open_dialog_save_mesh();
    }
  }
}


} // namespace viewer


#endif // MESHMENU_H_
