#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/named_params_helper.h>
#include <CGAL/boost/graph/properties.h>
#include <CGAL/property_map.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Weights/voronoi_region_weights.h>
#include <CGAL/Weights/cotangent_weights.h>

#include <CGAL/boost/graph/iterator.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <imgui.h>

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = CGAL::Point_3< Kernel >;
using Mesh = CGAL::Surface_mesh<Point>;

using vertex_descriptor =
  typename boost::graph_traits< Mesh >::vertex_descriptor;
using halfedge_descriptor =
  typename boost::graph_traits< Mesh >::halfedge_descriptor;
using edge_descriptor =
  typename boost::graph_traits< Mesh >::edge_descriptor;
using face_descriptor =
  typename boost::graph_traits< Mesh >::face_descriptor;

using Viewer = igl::opengl::glfw::Viewer;

namespace PMP = CGAL::Polygon_mesh_processing;


void update_local_mesh(Viewer& viewer, Mesh& mesh)
{
  mesh.clear();
  std::vector<Mesh::vertex_index> indices;
  indices.reserve(viewer.data().V.rows());
  for (size_t i = 0; i < viewer.data().V.rows(); ++i)
  {
    Eigen::RowVectorXd row = viewer.data().V.row(i);
    indices.push_back(mesh.add_vertex(Point(row[0], row[1], row[2])));
  }
  for (size_t i = 0; i < viewer.data().F.rows(); ++i)
  {
    Eigen::RowVectorXi row = viewer.data().F.row(i);
    size_t vd_0 = row[0];
    size_t vd_1 = row[1];
    size_t vd_2 = row[2];
    mesh.add_face(indices[vd_0], indices[vd_1], indices[vd_2]);
  }
}

void update_viewer_mesh(Viewer& viewer, const Mesh& mesh)
{
  Eigen::MatrixXd V;
  V.setZero(mesh.num_vertices(), 3);
  Eigen::MatrixXi F;
  F.setZero(mesh.num_faces(), 3);
  auto pmap = mesh.points();
  for (vertex_descriptor vd : vertices(mesh))
  {
    Point p = pmap[vd];
    V.row(vd)[0] = p[0];
    V.row(vd)[1] = p[1];
    V.row(vd)[2] = p[2];
  }
  for (face_descriptor fd : faces(mesh))
  {
    auto vs = CGAL::vertices_around_face(halfedge(fd, mesh), mesh).begin();
    F.row(fd)[0] = *vs++;
    F.row(fd)[1] = *vs++;
    F.row(fd)[2] = *vs;
  }
  viewer.data().clear();
  viewer.data().set_mesh(V, F);
}

void show_areas(Viewer& viewer, const Mesh& mesh)
{
  auto pmap = mesh.points();
  Eigen::VectorXd A;
  A.setZero(mesh.num_vertices());
  for (face_descriptor fd : faces(mesh))
  {
    auto vs = CGAL::vertices_around_face(halfedge(fd, mesh), mesh).begin();
    vertex_descriptor v_0 = *vs++;
    vertex_descriptor v_1 = *vs++;
    vertex_descriptor v_2 = *vs++;
    assert(v_0 == *v_2 && "Face not triangular");
    Point p_0 = pmap[v_0];
    Point p_1 = pmap[v_1];
    Point p_2 = pmap[v_2];
    A[v_0] += CGAL::Weights::voronoi_area(p_0, p_2, p_1);
    A[v_1] += CGAL::Weights::voronoi_area(p_1, p_0, p_2);
    A[v_2] += CGAL::Weights::voronoi_area(p_2, p_1, p_0);
  }
  viewer.data().set_data(A);
}

void isotropic_remeshing(Mesh& mesh, float target_edge_length)
{
  // Constrain edges with a dihedral angle over 60°
  unsigned int nb_iter = 3;

  PMP::isotropic_remeshing(faces(mesh),
                           target_edge_length, mesh,
                           PMP::parameters::number_of_iterations(nb_iter));
}

struct Gradient {
  Gradient(const Mesh &m) { init(m); }
  void init(const Mesh &mesh);
  std::vector<double> operator()(face_descriptor f, vertex_descriptor v);
  std::vector<double> data;
};

void Gradient::init(const Mesh &mesh) {}


int main(int argc, char *argv[]) {
  Mesh mesh;
  Viewer viewer;
  static bool local_mesh_dirty = true;
  igl::opengl::glfw::imgui::ImGuiPlugin plugin;
  igl::opengl::glfw::imgui::ImGuiMenu menu;

  viewer.plugins.push_back(&plugin);
  plugin.widgets.push_back(&menu);

  menu.callback_draw_viewer_menu = [&]()
  {
    menu.draw_viewer_menu();

    if (ImGui::CollapsingHeader("Mesh"))
    {
      static float target_edge_length = 0.035;
      ImGui::InputFloat("Edge length",
                        &target_edge_length,
                        0.005f, 0.005f, "%.3f");
      if (ImGui::Button("Isotropic remeshing"))
      {
        if (local_mesh_dirty)
          update_local_mesh(viewer, mesh);
        local_mesh_dirty = false;
        isotropic_remeshing(mesh, target_edge_length);
        update_viewer_mesh(viewer, mesh);
        return;
      }
    }

    if (ImGui::CollapsingHeader("Data"))
    {
      if (ImGui::Button("Areas"))
      {
        if (local_mesh_dirty)
          update_local_mesh(viewer, mesh);
        local_mesh_dirty = false;
        show_areas(viewer, mesh);
        return;
      }
    }
  };



  // auto vnormals = mesh.add_property_map<vertex_descriptor,
  // Vector>("v:normals", CGAL::NULL_VECTOR).first; auto fnormals =
  // mesh.add_property_map<face_descriptor, Vector>("f:normals",
  // CGAL::NULL_VECTOR).first;

  // PMP::compute_normals(mesh, vnormals, fnormals);

  // auto vareas_onering = mesh.add_property_map<vertex_descriptor,
  // K::FT>("v:areas_onering", K::FT(0)).first; auto vareas_voronoi =
  // mesh.add_property_map<vertex_descriptor, K::FT>("v:areas_voronoi",
  // K::FT(0)).first;

  // igl::opengl::glfw::Viewer viewer;
  // //viewer.data().set_mesh()

  // for (vertex_descriptor vd : vertices(mesh)) {
  //     K::FT area_onering =
  //     PMP::area(mesh.faces_around_target(mesh.halfedge(vd)), mesh);

  //     put(vareas_onering, vd, area_onering);
  // }
  // std::cout << "Vertex one-ring areas: " << std::endl;
  // for (vertex_descriptor vd : vertices(mesh)) {
  //     std::cout << get(vareas_onering, vd) << std::endl;
  // }

  // auto pmap = mesh.points();
  // CGAL::Weights::Secure_cotangent_weight_with_voronoi_area cotan{ mesh, pmap
  // };

  // PMP::face_border_length(typename
  // boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh
  // &pmesh)

  // CGAL::IO::write_polygon_mesh("my_bunny.off", mesh,
  // CGAL::parameters::vertex_normal_map(CGAL::get_property_map(CGAL::vertex_point,
  // mesh)));

  viewer.launch();

  return 0;
}
