#pragma once

#include <ostream>
#include <vector>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh {
    unsigned int NumberCell0D;
    vector <unsigned int> IdCell0D;
    vector <Vector2d> CoordinatesCell0D;
    map<unsigned int, list<unsigned int>> Cell0DMarkers;

    unsigned int NumberCell1D;
    vector <unsigned int> IdCell1D;
    vector <Vector2i> VerticesCell1D;
    map<unsigned int, list<unsigned int>> Cell1DMarkers;

    unsigned int NumberCell2D;
    vector <unsigned int> IdCell2D;
    vector <VectorXi> VerticesCell2D;
    vector <VectorXi> EdgesCell2D;
};
}
