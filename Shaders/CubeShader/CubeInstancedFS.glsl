//ƬԪ//ƬԪ��ɫ������
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
    FragColor = texture2DArray(ourTexture, data.TexCoord.stp);  //������ָ��λ�ý��в���
}