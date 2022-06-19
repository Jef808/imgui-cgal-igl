#include "Viewer/imgui/KeyHandler.h"

#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>

namespace ImGuiViewer {

bool KeyHandlerWidget::key_pressed(unsigned int unicode_key, int modifiers) {
  switch(unicode_key) {
    case '+':
    {
      float mult = 1.0+(1.)*0.05;
      const float min_zoom = 0.1f;
      viewer->core().camera_zoom = (viewer->core().camera_zoom * mult > min_zoom ? viewer->core().camera_zoom * mult : min_zoom);
      return true;
    }
    case '-':
    {
      float mult = 1.0+(-1.)*0.05;
      const float min_zoom = 0.1f;
      viewer->core().camera_zoom = (viewer->core().camera_zoom * mult > min_zoom ? viewer->core().camera_zoom * mult : min_zoom);
      return true;
    }
    default:
      return false;
  }
}


} // namespace ImGuiViewer
