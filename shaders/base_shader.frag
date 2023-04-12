#version 400 core

in vec3 fragTexCoords;

out vec4 fragColor;

uniform sampler2DArray uTexture;

void main()
{
	fragColor = texture(uTexture, fragTexCoords);
}