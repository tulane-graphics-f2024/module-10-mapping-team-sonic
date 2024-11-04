#version 150

in vec4 pos;
in vec4 N;

uniform samplerCube skybox;

out vec4 fragColor;

void main()
{
  
  fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    
}

