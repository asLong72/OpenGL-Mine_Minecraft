//������ɫ������
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;//��������
layout (location = 2) in mat4 modelIntance;

out vertex_info{
    vec3 normal;
    vec2 TexCoord;
} vertex;

uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * modelIntance * vec4 (aPos, 1.0f);
    vertex.TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}