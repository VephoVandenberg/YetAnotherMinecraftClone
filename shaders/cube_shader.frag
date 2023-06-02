#version 400 core

uniform sampler2DArray uTexture;

in vec3 fragTexCoords;
in vec3 fragNormals;
in vec3 fragCoords;
in float fragBrightness;

out vec4 outColor;

void main()
{
		outColor = fragBrightness * texture(uTexture, fragTexCoords);
}