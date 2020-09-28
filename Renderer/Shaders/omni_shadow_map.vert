#version 330

// Shader renders from the view of a light source
layout (location = 0) in vec3 pos;

uniform mat4 model;

void main() {
	gl_Position = model * vec4(pos, 1.0);
}