#include <string>

enum class UniformDataType
{
	Tex2D,
	TexCube,
	Mat4x4,
	Mat4x4Array,
	Mat3x3,
	Mat3x3Array,
	Vec4,
	Vec4Array,
	Vec3,
	Vec3Array,
	Vec2,
	Vec2Array,
	Float,
	FloatArray,
	Int,
	IntArray
};

struct ShaderUniform {
  const std::string name;
  UniformDataType dataType;
};
