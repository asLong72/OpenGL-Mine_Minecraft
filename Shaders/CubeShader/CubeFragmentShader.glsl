//片元着色器代码
#version 330 core

out vec4 FragColor;

in vertex_info{
    vec3 normal;
    vec2 TexCoord;
} vertex;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, vertex.TexCoord);  //对纹理指定位置进行采样
}