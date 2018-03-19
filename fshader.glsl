#version 460
struct Material
{
	vec3 diffuseColor;
	vec3 ambientColor;
};

uniform Material mat;
uniform vec3 lightDir;

smooth in vec3 normal;

out vec4 fColor;

void main()
{
    // Set fragment color from texture (diffuse with a touch of ambient)
	vec3 color = dot(lightDir, normal) * mat.diffuseColor + mat.ambientColor;
	fColor = vec4(color, 1.0);
}