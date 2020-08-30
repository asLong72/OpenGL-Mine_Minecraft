//顶点着色器代码
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;//颜色
layout (location = 2) in vec2 aTexCoord;//纹理坐标

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4 (aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}