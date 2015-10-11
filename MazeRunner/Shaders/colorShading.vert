#version 400

in vec4 vertexPosition;

out vec4 fragmentColor;

uniform mat4 MVP;
uniform vec4 COLOR;

void main()
{
	gl_Position = MVP * vertexPosition;
	fragmentColor = COLOR;
}