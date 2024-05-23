#include "Mesh.h"

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

const float* Mesh::GetVertices(int &out_numVertices) {
    out_numVertices = numVertices;
    return vertices;
}

const unsigned int* Mesh::GetIndices(int &out_numIndices) {
    out_numIndices = numIndices;
    return indices;
}

Mesh::~Mesh() {
    // ClearMesh();
    delete vertices;
    delete indices;
}
