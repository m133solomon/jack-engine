#version 410 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D[16] u_Textures;

void main()
{
    vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
    o_Color = texColor * v_Color;
}

