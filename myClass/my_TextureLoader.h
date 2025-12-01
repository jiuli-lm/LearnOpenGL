#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <stb/stb_image.h>

class Texture{
public:
    GLuint ID;
    int width, height, nrChannels;

    // 这里考虑的opnegl的坐标系与图片坐标系的不同（opengl坐标原点位于左下角 大多数图片第一个像素在左上角）
    Texture(const std::string& path, bool flip = true) {
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if(!data){
            std::cerr << "Failed to load texture:" << path << std::endl;
            return;
        }
        // 根据图片通道数选择纹理存储格式 4通道->RGBA 3通道->RGB 1通道->RED(灰度图)
        GLenum format = (nrChannels == 4) ? GL_RGBA : (nrChannels == 3 ? GL_RGB : GL_RED);

        // 生成纹理
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    void use(GLuint textureUnit = 0) const {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

     ~Texture() {
        if (ID != 0)
            glDeleteTextures(1, &ID);
    }

    // 禁止拷贝，支持移动
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept : ID(other.ID), width(other.width), height(other.height), nrChannels(other.nrChannels) {
        other.ID = 0;
    }
    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &ID);
            ID = other.ID;
            width = other.width;
            height = other.height;
            nrChannels = other.nrChannels;
            other.ID = 0;
        }
        return *this;
    }

};

#endif