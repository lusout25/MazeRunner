#version 400

in vec4 vertexPosition;
in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 MVP;
uniform vec4 COLOR;

void main()
{
	gl_Position = MVP * vertexPosition;
	fragmentColor = COLOR;
	fragmentUV = vertexUV;
}