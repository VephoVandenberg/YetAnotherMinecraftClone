#version 330 core

in vec3 fragTexCoords;

out vec4 fragColor;

uniform samplerCube uTexture;

void main()
{
	fragColor = texture(uTexture, fragTexCoords);
}