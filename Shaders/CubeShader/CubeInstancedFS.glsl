//片元//片元着色器代码
#version 330 core
#extension GL_EXT_texture_array : enable

out vec4 FragColor;

in DATA
{
    vec3 TexCoord;
    vec3 normal;
} data;

uniform sampler2DArray ourTexture;

void main()
{
    FragColor = texture2DArray(ourTexture, data.TexCoord.stp);  //对纹理指定位置进行采样
}