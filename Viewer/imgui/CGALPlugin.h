#ifndef CGALPLUGIN_H_
#define CGALPLUGIN_H_

#include <CGAL/Surface_mesh/Surface_mesh_fwd.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>

#include "igl/opengl/glfw/imgui/ImGuiPlugin.h"
#include <igl/opengl/glfw/Viewer.h>

#include <string>

namespace ImGuiViewer {


template < typename Point >
class ImGuiCGALPlugin
    : public igl::opengl::glfw::imgui::ImGuiPlugin {
public:
  using Mesh = CGAL::Surface_mesh< Point >;
  /**
   * Launch a file explorer prompt in the CGLA meshes directory.
   */
  bool load(std::string filename) override;

  /**
   * Scrolling doesn't work so well with the trackpad
   */
  bool mouse_scroll(float delta_y) override;

  std::function<std::string(const std::string &)> load_filename_callback;

private:
  Mesh mesh;
  std::string m_current_mesh_name;

  /**
   * Store the mesh data into Eigen matrices for compatibility with libigl
   */
  void CGAL2IGL(const Mesh &mesh,
                Eigen::MatrixXd &vertices_out,
                Eigen::MatrixXi &faces_out);
};

template < typename Point >
inline bool ImGuiCGALPlugin< Point >::load(std::string filename) {
  Mesh tmp_mesh;

  if (not load_filename_callback) {
    load_filename_callback = [](const auto &s) { return s; };
  }

  static std::string full_path;
  full_path = load_filename_callback(filename);
  bool ok = CGAL::IO::read_polygon_mesh(full_path, tmp_mesh);

  if (ok) {
    m_current_mesh_name = filename;
    std::swap(tmp_mesh, mesh);
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    CGAL2IGL(mesh, V, F);
    viewer->data().clear();
    viewer->data().set_mesh(V, F);
  }
  return ok;
}

template < typename Point >
inline bool ImGuiCGALPlugin< Point >::mouse_scroll(float) {
  return true;
}

template < typename Point >
inline void ImGuiCGALPlugin< Point >::CGAL2IGL(const Mesh &mesh,
                                                Eigen::MatrixXd &vertices_out,
                                                Eigen::MatrixXi &faces_out) {
  using vertex_descriptor =
    typename boost::graph_traits< Mesh >::vertex_descriptor;
  using face_descriptor =
    typename boost::graph_traits< Mesh >::face_descriptor;

  vertices_out.resize(mesh.number_of_vertices(), 3);
  faces_out.resize(mesh.number_of_faces(), 3);

  auto pmap = mesh.points();

  for (vertex_descriptor v : vertices(mesh)) {
    vertices_out.row(v)[0] = pmap[v][0];
    vertices_out.row(v)[1] = pmap[v][1];
    vertices_out.row(v)[2] = pmap[v][2];
  }
  for (face_descriptor fd : faces(mesh)) {
    auto vs =
        CGAL::vertices_around_face(CGAL::halfedge(fd, mesh), mesh).begin();
    faces_out.row(fd)[0] = *vs++;
    faces_out.row(fd)[1] = *vs++;
    faces_out.row(fd)[2] = *vs;
  }
}

} // namespace ImGuiViewer

#endif // CGALPLUGIN_H_
