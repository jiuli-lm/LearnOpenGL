#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;

void main(){
    FragColor = mix(texture(texture_2,TexCoord), texture(texture_1,TexCoord), 0.2);
}