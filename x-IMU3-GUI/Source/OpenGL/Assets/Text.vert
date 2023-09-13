#version 330 core

in vec3 vertexIn;
in vec2 textureIn;

out vec2 textureCoord_frag;

uniform mat4 transform;

void main()
{
    textureCoord_frag = textureIn;
    gl_Position = transform * vec4(vertexIn, 1.0);
}