//ƬԪ��ɫ������
#version 330 core

out vec4 FragColor;

in vertex_info{
    vec3 normal;
    vec2 TexCoord;
} vertex;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, vertex.TexCoord);  //������ָ��λ�ý��в���
}