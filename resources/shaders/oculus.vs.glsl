#version 410 core

uniform mat4 posMatrix;
uniform mat4 texMatrix;

in vec2 position;
in vec2 texCoord;

out vec2 oTexCoord;

void main() {
	gl_Position = posMatrix * vec4(position, 0.0, 1.0); //posMatrix * vec4(position, 0.0, 1.0);
	oTexCoord = vec2(texMatrix * vec4(texCoord, 0.0, 1.0));
}

// // view is to transform input vertex into clip space (-1, 1)
// uniform mat4 View;
// // Texm is to transform texture cooridate into oculus rift screen (half screen size (0, 0.5) (0.5, 1))
// uniform mat4 Texm;

// // input vertex for drawing shape 
// layout (location = 0) in vec4 Position;
// // input texture cooridate for oculus to apply distortion later
// // input texture cooridate will be the x, y element of the input texture
// layout (location = 1) in vec2 TexCoord;

// out vec2 oTexCoord;

// void main() {
//     gl_Position = View * Position;
//     oTexCoord = vec2(Texm * vec4(TexCoord, 0, 1));
//     oTexCoord.y = 1.0 - oTexCoord.y;
// }
