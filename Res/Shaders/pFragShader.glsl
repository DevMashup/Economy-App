#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D ourTexture;

const float weights[5] = float[5](0.2, 0.2, 0.2, 0.2, 0.2);
//0.06136, 0.24477, 0.38774, 0.24477, 0.06136

void main()
{
    vec3 result = vec3(0.0);
    vec2 texelSize = 25.0 / textureSize(ourTexture, 0);

    // Apply horizontal blur
    for(int i = -2; i <= 2; i++)
    {
        result += texture(ourTexture, texCoords + vec2(float(i) * texelSize.x, 0)).rgb * weights[i + 2];
    }

    // Normalize
    result /= 1.0; // Assuming sum of weights is 1.0

    // Reset result for vertical blur
    result = vec3(0.0);

    // Apply vertical blur
    for(int i = -2; i <= 2; i++)
    {
        result += texture(ourTexture, texCoords + vec2(float(i) * texelSize.y, 0)).rgb * weights[i + 2];
    }

    // Normalize
    result /= 1.0; // Assuming sum of weights is 1.0

    FragColor = vec4(result, 1.0);
}