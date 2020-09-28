#version 330
layout (location = 0) in vec3 pos; 
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tan;

out vec4 vCol;
out vec2 texCoord;
out vec3 fragPos;
out vec4 directionalLightSpacePos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightSpaceTransform;


mat3 CalcTBN() {
	// calc bitangent
	vec3 bitangent = normalize(cross(norm, tan));
	vec3 T = normalize(vec3(model * vec4(tan, 0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(norm, 0.0)));
	return mat3(T, B, N);
}

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
	directionalLightSpacePos = directionalLightSpaceTransform * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	texCoord = tex;
	fragPos = (model * vec4(pos, 1.0)).xyz;
	
	TBN = CalcTBN();
}