#version 400 

uniform mat4 MVP;
uniform mat4 normalMatrix; 

layout (location = 0) in vec3 inPosition; 
layout (location = 1) in vec4 inColor; 
layout (location = 2) in vec3 inNormal; 

out vec4 fragmentColor; 

smooth out vec3 vNormal; 

void main() 
{ 
   gl_Position = MVP * vec4(inPosition, 1.0); 
   fragmentColor = inColor; 
   vec4 vRes = normalMatrix*vec4(inNormal, 0.0); 
   vNormal = vRes.xyz; 
}