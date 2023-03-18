#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 inTexCoords;

out vec2 fragTexCoords;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
}