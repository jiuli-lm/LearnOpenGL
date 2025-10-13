#version 330 core

out vec4 FragColor;
in vec3 myColor;

//uniform vec4 myColor;

void main() {
    //颜色空间(红 绿 蓝 alph)
    FragColor = vec4(myColor,1.0);    
}