#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
	FragColor = texture(ourTexture, texCoords);
}