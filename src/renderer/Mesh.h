#pragma once

#include <GL/glew.h>

#include "Material.h"

class Mesh {
public:
    Mesh();

    void CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);

    const float* GetVertices(int &out_numVertices);
    const unsigned int* GetIndices(int &out_numIndices);
    
    ~Mesh();

private:
    float *vertices;
    unsigned int *indices;
    unsigned int numVertices;
    unsigned int numIndices;
};
