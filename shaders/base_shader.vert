#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 fragTexCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0f);
	fragTexCoords = inTexCoords;
}