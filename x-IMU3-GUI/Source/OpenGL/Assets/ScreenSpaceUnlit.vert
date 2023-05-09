// Used for objects not affected by lighting
#version 330 core

layout (location = 0) in vec3 position_vert;
layout (location = 2) in vec2 textureCoord_vert;

out vec2 textureCoord_frag;

uniform mat4 modelViewProjectionMatrix;

uniform float inverseScreenScale = 1.0f;

void main()
{
    gl_Position = modelViewProjectionMatrix * vec4 (position_vert * inverseScreenScale, 1.0);
    textureCoord_frag = textureCoord_vert;
}
