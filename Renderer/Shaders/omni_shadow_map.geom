#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

// matrix in the direction of each cube face
uniform mat4 lightMatrices[6];

out vec4 fragPos;

void main() {
	for (int face = 0; face < 6; face++) {
		gl_Layer = face;
		// process triangles, turn into triangle strips
		for (int i = 0; i < 3; i++) {
			fragPos = gl_in[i].gl_Position;
			gl_Position = lightMatrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}