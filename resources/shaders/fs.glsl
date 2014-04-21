#version 410 core

in VS_OUT {
    vec4 colour;
} fs_in;

out vec4 colour;

void main()
{
    colour = fs_in.colour;
}
