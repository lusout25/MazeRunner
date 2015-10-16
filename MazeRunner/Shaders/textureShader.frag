#version 400

in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 fragColor;

uniform sampler2D SAMPLER;
uniform bool USETEXTURE;

void main()
{
	vec4 textureColor;
	
	if (USETEXTURE) {
	 textureColor = texture(SAMPLER, fragmentUV);
	} else {
	 textureColor = vec4(1, 1, 1, 1);
	}

	fragColor = textureColor * fragmentColor;
}