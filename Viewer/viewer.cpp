#include "viewer.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include "Viewer/imgui/CGALPlugin.h"
#include "Viewer/imgui/MeshMenu.h"
#include "Viewer/imgui/KeyHandler.h"


namespace ImGuiViewer {


void init(igl::opengl::glfw::Viewer* viewer) {
  using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
  using Point = Kernel::Point_3;
  using Mesh = CGAL::Surface_mesh< Point >;

  static ImGuiCGALPlugin< Point > plugin;
  static MeshMenuWidget< Point > menu;
  static KeyHandlerWidget key_handler;

  viewer->plugins.push_back(&plugin);
  plugin.widgets.push_back(&menu);
  plugin.widgets.push_back(&key_handler);

  menu.set_style_callback = [](){
    ImVec2 WindowSize = ImGui::GetWindowSize();
    ImVec2 FramePadding = ImVec2(6.0f, 6.0f);
    ImVec2 UIPanelSize{ WindowSize.x / 6, WindowSize.y };
    ImVec2 DisplayPanelSize{ WindowSize.x / 2, WindowSize.y };
    ImVec2 LogPanelSize{ WindowSize.x / 3, WindowSize.y };
    ImGui::GetStyle().Alpha = 0.8f;
    ImGui::GetStyle().FramePadding = FramePadding;
    ImGui::SetNextWindowSize(UIPanelSize, ImGuiCond_FirstUseEver);
  };
}

} // namespace ImGuiViewer
