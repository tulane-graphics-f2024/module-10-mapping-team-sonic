#version 150

in  vec4 vPosition;

uniform mat4 ModelView;
uniform mat4 Projection;

out vec3 TexCoords;

void main()
{
    TexCoords = vPosition.xyz;
    vec4 pos = ModelView*Projection * vPosition;
    gl_Position = pos.xyww;
}  

