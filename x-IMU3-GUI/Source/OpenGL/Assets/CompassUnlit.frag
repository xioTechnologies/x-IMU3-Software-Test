#version 330 core

in vec2 textureCoord_frag;

out vec4 fragColour;

uniform sampler2D textureImage;
uniform vec4 tint;
uniform vec3 northColour;

void main()
{
    vec4 textureColour = texture(textureImage, textureCoord_frag);

    // If colour in texture is close enough pure red, replace with new northColour
    const vec3 redInCompassImage = vec3(1.0f, 0.0f, 0.0f);
    textureColour.rgb = (0.7 > distance(textureColour.rgb, redInCompassImage)) ? northColour : textureColour.rgb;

    fragColour = textureColour * tint;
}