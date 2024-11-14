#version 150

uniform vec4 ambient;
uniform vec4 LightPosition;

in vec4 pos;
in vec4 N;
in vec2 texCoord;

uniform mat4 ModelViewLight;

uniform sampler2D textureEarth;
uniform sampler2D textureNight;
uniform sampler2D textureCloud;
uniform sampler2D texturePerlin;

uniform float animate_time;


out vec4 fragColor;

void main()
{
   // float time = animate_time * 0.3;
   // LightPosition = vec4(cos(time) * LightPosition.x, LightPosition.y, sin(time) * LightPosition.z, 1.0);
    
    
    vec4 L = normalize( (ModelViewLight*LightPosition) - pos );
    float D = max(dot(normalize(N), L), 0.0);
    float Kd = 1.0;
  

  
    vec4 earth = texture(textureEarth, texCoord);
    vec4 clouds = texture(textureCloud, texCoord);
    vec4 night = texture(textureNight, texCoord);
    earth = Kd*earth;

    
    vec4 dayCycle = mix(night, earth, D);
    vec4 diffuse = D * dayCycle;
    vec4 blend = clamp(earth + clouds, 0.0, 1.0);
  
  fragColor = ambient + blend;
  fragColor = clamp(fragColor, 0.0, 1.0);
  fragColor.a = 1.0;
}


