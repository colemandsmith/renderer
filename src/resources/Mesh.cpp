#include "Mesh.h"

#include <stdio.h>

Mesh::Mesh() {
    vertices = nullptr;
    indices = nullptr;
    numVertices = 0;
    numIndices = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
    this->vertices = vertices;
    this->indices = indices;
    this->numVertices = numOfVertices;
    this->numIndices = numOfIndices;
}

float* Mesh::GetVertices(int &out_numVertices) const {
    out_numVertices = numVertices;
    return vertices;
}

unsigned int* Mesh::GetIndices(int &out_numIndices) const {
    out_numIndices = numIndices;
    return indices;
}

Mesh::~Mesh() {
    // ClearMesh();
    delete vertices;
    delete indices;
}
