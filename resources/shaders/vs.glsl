#version 410 core

in vec4 position;

out VS_OUT {
    vec4 colour;
} vs_out;

uniform mat4 a;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
    gl_Position = projection_matrix * model_view_matrix * position;
    vs_out.colour = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}