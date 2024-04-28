#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

bool ImportCell0Ds (const string &fileName, PolygonalMesh& mesh);

bool ImportCell1Ds (const string &fileName, PolygonalMesh& mesh);

bool ImportCell2Ds (const string &fileName, PolygonalMesh& mesh);

/* This function imports the data from all the files, prints the marker for confirmation of correct storage and
runs the tests of properties (including the non-zero lenght of the edges and the non-zero area of the polygons */
bool ImportMesh (const string &filePath, PolygonalMesh& mesh);
}
