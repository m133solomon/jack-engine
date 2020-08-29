#version 330 core

in vec2 v_TexCoord;
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * u_Color;
}

