#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoords;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in float aBrightness;

out vec3 fragTexCoords;
out vec3 fragNormals;
out vec3 fragCoords;
out float fragBrightness;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0f);
	fragTexCoords = aTexCoords;
	fragCoords = vec3(u_model * vec4(aPos, 1.0f));
	fragNormals = aNormals;
	fragBrightness = aBrightness;
}