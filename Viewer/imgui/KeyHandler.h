#ifndef KEYHANDLER_H_
#define KEYHANDLER_H_

#include <igl/opengl/glfw/imgui/ImGuiWidget.h>


namespace ImGuiViewer {

  /**
 * The widget handling keyboard inputs.
 */
class KeyHandlerWidget : public igl::opengl::glfw::imgui::ImGuiWidget {
public:

    KeyHandlerWidget()
        : ImGuiWidget()
    {
        name = "key_handler";
    }

    /**
     * Handle the events associated to keyboard inputs in a switch statement.
     */
    bool key_pressed(unsigned int unicode_key, int modifiers) override;
};


} // namespace ImGuiViewer

#endif // KEYHANDLER_H_
