#version 450

in vec4 vPosition;
in vec4 vColor;
out vec4 color;
uniform int CCSToNDCS;
uniform mat4 uModelViewMatrix;
uniform mat4 SceneViewMatrix;


void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  if(CCSToNDCS == 1)
  {
  	gl_Position.x /= gl_Position.w;
  	gl_Position.y /= gl_Position.w;
  	gl_Position.z /= gl_Position.w;
  	gl_Position.w = 1;
  }
  color = vColor;
}
