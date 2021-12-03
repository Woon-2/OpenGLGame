#version 330 core

out vec4 frag_color;
in vec3 out_Color;

void main()
{
    frag_color = vec4(out_Color, 1.0);
}