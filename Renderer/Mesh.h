#pragma once

#include <GL/glew.h>

#include "Material.h"

class Mesh {

public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void SetMaterial(Material* material);
    
    void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
    Material* material;
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};