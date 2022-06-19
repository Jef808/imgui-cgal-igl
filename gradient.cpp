#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Weights/cotangent_weights.h>

#include <igl/opengl/glfw/Viewer.h>

#include "Viewer/viewer.h"

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = Kernel::Point_3;
using Vector = Kernel::Vector_3;
using Mesh = CGAL::Surface_mesh< Point >;

typedef boost::graph_traits<Mesh>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;


constexpr const char* default_mesh = "elephant.off";


struct Gradient {
    Gradient(const Mesh& m) {
        init(m);
    }

    void init(const Mesh& mesh);

    template<typename F>
    std::vector<double> operator()(F f);

    std::vector<double> data;
};

void Gradient::init(const Mesh& mesh) {

}


using namespace std::chrono_literals;

int main(int argc, char *argv[]) {

    igl::opengl::glfw::Viewer viewer;

    ImGuiViewer::init(&viewer);

    viewer.launch();

    // std::cout << "Have " << mesh.vertices().size() << " vertices and " << mesh.faces().size() << " faces." << std::endl;

    // auto vnormals = mesh.add_property_map<vertex_descriptor, Vector>("v:normals", CGAL::NULL_VECTOR).first;
    // auto fnormals = mesh.add_property_map<face_descriptor, Vector>("f:normals", CGAL::NULL_VECTOR).first;

    // PMP::compute_normals(mesh, vnormals, fnormals);

    // auto vareas_onering = mesh.add_property_map<vertex_descriptor, K::FT>("v:areas_onering", K::FT(0)).first;
    // auto vareas_voronoi = mesh.add_property_map<vertex_descriptor, K::FT>("v:areas_voronoi", K::FT(0)).first;

    // igl::opengl::glfw::Viewer viewer;
    // //viewer.data().set_mesh()

    // for (vertex_descriptor vd : vertices(mesh)) {
    //     K::FT area_onering = PMP::area(mesh.faces_around_target(mesh.halfedge(vd)), mesh);

    //     put(vareas_onering, vd, area_onering);
    // }
    // std::cout << "Vertex one-ring areas: " << std::endl;
    // for (vertex_descriptor vd : vertices(mesh)) {
    //     std::cout << get(vareas_onering, vd) << std::endl;
    // }

    // auto pmap = mesh.points();
    // CGAL::Weights::Secure_cotangent_weight_with_voronoi_area cotan{ mesh, pmap };

    //PMP::face_border_length(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh &pmesh)

    //CGAL::IO::write_polygon_mesh("my_bunny.off", mesh, CGAL::parameters::vertex_normal_map(CGAL::get_property_map(CGAL::vertex_point, mesh)));

    return 0;

}
