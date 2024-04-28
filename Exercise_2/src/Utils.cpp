#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <Eigen/Eigen>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

namespace PolygonalLibrary {

bool ImportCell0Ds (const string &fileName, PolygonalMesh& pmesh){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    list<string> listLines;
    string line;
    while(getline(file,line)){
        listLines.push_back(line);
    }
    file.close();
    listLines.pop_front();

    pmesh.NumberCell0D = listLines.size();
    pmesh.IdCell0D.reserve(pmesh.NumberCell0D);
    pmesh.CoordinatesCell0D.reserve(pmesh.NumberCell0D);

    for(const string &riga : listLines){
        istringstream ssRiga(riga);

        unsigned int id;
        unsigned int marker;
        char c1,c2,c3;
        Vector2d coord;

        ssRiga >> id >> c1 >> marker >> c2 >> coord(0) >> c3 >> coord(1);

        pmesh.IdCell0D.push_back(id);
        pmesh.CoordinatesCell0D.push_back(coord);

        if(marker != 0){
            auto additional = pmesh.Cell0DMarkers.insert({marker, {id}});
            if(!additional.second){
                (additional.first)->second.push_back(id);
            }
        }
    }
    file.close();
    return true;
}



bool ImportCell1Ds (const string &fileName, PolygonalMesh& pmesh){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    list<string> listLines;
    string line;
    while(getline(file,line)){
        listLines.push_back(line);
    }
    file.close();
    listLines.pop_front();

    pmesh.NumberCell1D = listLines.size();
    pmesh.IdCell1D.reserve(pmesh.NumberCell1D);
    pmesh.VerticesCell1D.reserve(pmesh.NumberCell1D);

    for(const string &riga : listLines){
        istringstream ssRiga(riga);

        unsigned int id;
        unsigned int marker;
        char c1,c2,c3;
        Vector2i vertex;

        ssRiga >> id >> c1 >> marker >> c2 >> vertex(0) >> c3 >> vertex(1);

        pmesh.IdCell1D.push_back(id);
        pmesh.VerticesCell1D.push_back(vertex);

        if(marker != 0){
            auto additional = pmesh.Cell1DMarkers.insert({marker, {id}});
            if(!additional.second){
                (additional.first)->second.push_back(id);
            }
        }
    }
    file.close();
    return true;
}



bool ImportCell2Ds (const string &fileName, PolygonalMesh& pmesh){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    list<string> listLines;
    string line;
    while(getline(file,line)){
        listLines.push_back(line);
    }
    file.close();
    listLines.pop_front();

    pmesh.NumberCell2D = listLines.size();
    pmesh.IdCell2D.reserve(pmesh.NumberCell2D);
    pmesh.VerticesCell2D.reserve(pmesh.NumberCell2D);
    pmesh.EdgesCell2D.reserve(pmesh.NumberCell2D);

    for(const string &riga : listLines){
        istringstream ssRiga(riga);
        string r;
        unsigned int id;
        unsigned int marker;
        unsigned int numVert;
        unsigned int numEdges;
        VectorXi vertices;
        VectorXi edges;

        getline(ssRiga, r, ';'); id = stoi(r);
        getline(ssRiga, r, ';'); marker = stoi(r);
        getline(ssRiga, r, ';'); numVert = stoi(r);
        vertices.resize(numVert);
        for (unsigned int i=0; i<numVert; i++){
            getline(ssRiga, r, ';'); vertices(i) = stoi(r);
        }
        getline(ssRiga, r, ';'); numEdges = stoi(r);
        edges.resize(numEdges);
        if (numVert != numEdges){
            continue;
        }
        for (unsigned int i=0; i<numVert-1; i++){
            getline(ssRiga, r, ';'); edges(i) = stoi(r);
        }
        getline(ssRiga,r); edges(numVert-1) = stoi(r);

        pmesh.IdCell2D.push_back(id);
        pmesh.VerticesCell2D.push_back(vertices);
        pmesh.EdgesCell2D.push_back(edges);
    }
    file.close();
    return true;
}



bool ImportMesh (const string &filePath, PolygonalMesh& pmesh){
    if (!ImportCell0Ds(filePath + "/Cell0Ds.csv", pmesh)){
        return false;
    }
    else {
        cout << "Cell0D marker:" << endl;
        for(auto iter = pmesh.Cell0DMarkers.begin(); iter != pmesh.Cell0DMarkers.end(); iter++) {
            cout << "key:\t" << iter -> first << "\t values:";
            for(const unsigned int id : iter -> second){
                cout << "\t" << id;
            }
            cout << endl;
        }
    }


    if (!ImportCell1Ds(filePath + "/Cell1Ds.csv", pmesh)){
        return false;
    }
    else {
        cout << "Cell1D marker:" << endl;
        for(auto iter = pmesh.Cell1DMarkers.begin(); iter != pmesh.Cell1DMarkers.end(); iter++) {
            cout << "key:\t" << iter -> first << "\t values:";
            for(const unsigned int id : iter -> second){
                cout << "\t" << id;
            }
            cout << endl;
        }
    }

    //The vertices in Cell1Ds file must also exist in Cell0Ds
    for (unsigned int h=0; h<pmesh.NumberCell1D; h++){
        Vector2i id1D = pmesh.VerticesCell1D[h];
        for (unsigned int l=0; l<2; l++){
            if (find(pmesh.IdCell0D.begin(), pmesh.IdCell0D.end(), id1D[l]) == pmesh.IdCell0D.end()){
                cerr << "Not existing Id from Cell1Ds in Cell0Ds" << endl;
            }
        }
    }


    if (!ImportCell2Ds(filePath + "/Cell2Ds.csv", pmesh)){
        return false;
    }

    /* The vertices in Cell2Ds file must also exist in Cell0Ds
     * and the edges in Cell2Ds file must also exist in Cell1Ds */
    for (unsigned int l=0; l<pmesh.NumberCell2D; l++){
        VectorXi id2D = pmesh.VerticesCell2D[l];
        for (unsigned int h=0; h<id2D.size(); h++){
            if (find(pmesh.IdCell0D.begin(), pmesh.IdCell0D.end(), id2D[h]) == pmesh.IdCell0D.end()){
                cerr << "Not existing Id form Cell2Ds in Cell0Ds" << endl;
            }
        }
    }
    for (unsigned int j=0; j<pmesh.NumberCell2D; j++){
        VectorXi id2D_ed = pmesh.EdgesCell2D[j];
        for (unsigned int k=0; k<id2D_ed.size(); k++){
            if (find(pmesh.IdCell1D.begin(), pmesh.IdCell1D.end(), id2D_ed[k]) == pmesh.IdCell1D.end()){
                cerr << "Not existing Id form Cell2Ds in Cell1Ds" << id2D_ed[k] << endl;
            }
        }
    }


    // ------ TEST ------
    // The edges of polygons have non-zero lenght
    double tol = 10 * numeric_limits<double>::epsilon();
    for (unsigned int i=0; i<pmesh.NumberCell1D; i++){
        Vector2i vertex = pmesh.VerticesCell1D[i];
        Vector2d coord_origin = pmesh.CoordinatesCell0D[vertex(0)];
        Vector2d coord_end = pmesh.CoordinatesCell0D[vertex(1)];
        if(sqrt(pow(coord_end(1)-coord_origin(1),2)+pow(coord_end(0)-coord_origin(0),2)) < tol){
            cerr << "The edge of id " << i << " has zero lenght" << endl;
        }
    }
    // ------ TEST ------


    // ------ TEST ------
    // The area of the polygons is non-zero
    double tol_edges = 10 * numeric_limits<double>::epsilon();
    double toll = (tol_edges*tol_edges)*sqrt(3)/4;
    for (unsigned int j=0; j<pmesh.NumberCell2D; j++){
        VectorXi vert_poly = pmesh.VerticesCell2D[j];
        unsigned int vp = vert_poly.size();
        MatrixXd coord_poly(2,vp);
        for (unsigned int k=0; k<vp; k++){
            coord_poly.col(k) << pmesh.CoordinatesCell0D[vert_poly(k)];
        }
        double A2 = 0;
        for (unsigned int i=0; i<vp; i++){
            A2 += coord_poly(0,i)*coord_poly(1,(i+1)%vp) - coord_poly(0,(i+1)%vp)*coord_poly(1,i);
        }
        double Area = abs(A2)/2;
        if (Area < toll){
            cerr << "The polygon of id " << j << " has area zero" << endl;
        }
    }
    // ------ TEST ------

    return true;
}
}
