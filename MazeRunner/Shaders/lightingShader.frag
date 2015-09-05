#version 400

in vec4 fragmentColor;

smooth in vec3 vNormal; 

out vec4 outputColor; 


void main() 
{ 
	vec3 vColor = vec3(1.0, 1, 1);
	vec3 vDirection = normalize(vec3(1, 1, 1));
	float fAmbientIntensity = 0.1;

   float fDiffuseIntensity = max(0.0, dot(normalize(vNormal), -vDirection)); 

   outputColor = fragmentColor * vec4(vColor * (fAmbientIntensity + fDiffuseIntensity), 1); 
}