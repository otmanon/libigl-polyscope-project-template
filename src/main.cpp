#include "polyscope/polyscope.h"

#include <igl/PI.h>
#include <igl/avg_edge_length.h>
#include <igl/barycenter.h>
#include <igl/boundary_loop.h>
#include <igl/exact_geodesic.h>
#include <igl/gaussian_curvature.h>
#include <igl/invert_diag.h>
#include <igl/lscm.h>
#include <igl/massmatrix.h>
#include <igl/per_vertex_normals.h>
#include <igl/readOBJ.h>

#include "polyscope/point_cloud.h"
#include "polyscope/surface_mesh.h"

#include <iostream>
#include <unordered_set>
#include <utility>
#include <map>

#include "polyscope/persistent_value.h"
#include "polyscope/render/engine.h"
#include "polyscope/transformation_gizmo.h"
#include "polyscope/utilities.h"
#include "polyscope/widget.h"

// The mesh, Eigen representation
Eigen::MatrixXd meshV;
Eigen::MatrixXi meshF;

using namespace Eigen;

using namespace polyscope;

void my_callback()
{
	meshV += 0.001*Eigen::MatrixXd::Ones(meshV.rows(), meshV.cols());
	polyscope::getSurfaceMesh("input mesh")->updateVertexPositions(meshV);

	std::cout << glm::to_string(state::gizmos["my_gizmo"]->T) << std::endl;
	
}

int main(int argc, char **argv) {
  // Configure the argument parser


//	
  polyscope::options::autocenterStructures = true;
  polyscope::view::windowWidth = 1024;
  polyscope::view::windowHeight = 1024;

  // Initialize polyscope
  polyscope::init();
  //PersistentValue<glm::mat4> objectTransform("id", glm::mat4(1.0));
  PersistentValue<glm::mat4> objectTransform("id", glm::mat4(1.0));
  // Options
  std::string filename = "../spot.obj";
  std::cout << "loading: " << filename << std::endl;

  // Read the mesh
  igl::readOBJ(filename, meshV, meshF);

  // Register the mesh with Polyscope
  auto mesh = polyscope::registerSurfaceMesh("input mesh", meshV, meshF);



  Eigen::VectorXd C = Eigen::MatrixXd::Ones(meshV.rows(), 1);
 
  mesh->addVertexScalarQuantity("random data", C);
  // Show the gui
 // std::set<Widget>::iterator it;
  TransformationGizmo gizmo("my_gizmo", objectTransform.get(), &objectTransform);
  //std::initializer_list<Widget> list(TransformationGizmo("my_gizmo", objectTransform.get(), &objectTransform));
  //polyscope::state::widgets.insert(it, &gizmo);
  gizmo.enabled = true;
	state::widgets.insert(&gizmo);
//polyscope::state::gizmos.emplace("my_gizmo", &gizmo);
	state::gizmos["my_gizmo"] = &gizmo;
	// Add the callback
	polyscope::state::userCallback = my_callback;
  polyscope::show();

  
 
  return 0;
}
