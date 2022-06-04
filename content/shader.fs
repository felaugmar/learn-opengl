#version 330 core

out vec4 fragment_color;

in vec4 vertex_color;

uniform float multiply;

void main()
{
   fragment_color = vertex_color * multiply;
}
