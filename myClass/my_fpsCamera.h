#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum FpsCamera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//默认摄像机值
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class FpsCamera{
public:
    // 相机的属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // 欧拉角
    float Yaw;
    float Pitch;
    // 相机设置
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // 向量构造函数
    FpsCamera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW, float pitch = PITCH) 
        : Front(glm::vec3(0.0f,0.0f,-1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // 标量构造函数
    FpsCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : Front(glm::vec3(0.0f,0.0f,-1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX,posY,posZ);
        WorldUp = glm::vec3(upX,upY,upZ);
        Yaw = yaw;
        Pitch =  pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position, Position + Front, Up);
    }
    // 输入
    void ProcessKeyboard(FpsCamera_Movement direction, float deltaTime){
        float velocity = MovementSpeed * deltaTime;
        if(direction == FORWARD)  Position += Front * velocity;
        if(direction == BACKWARD) Position -= Front * velocity;
        if(direction == RIGHT)    Position += Right * velocity;
        if(direction == LEFT)     Position -= Right * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true){
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if(constrainPitch){
            if(Pitch >  89.0f) Pitch =  89.0f;
            if(Pitch < -89.0f) Pitch = -89.0f;
        }
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset){
        Zoom -= (float)yoffset;
        if(Zoom <  1.0f) Zoom =  1.0f;
        if(Zoom > 45.0f) Zoom = 45.0f;
    }

private:
    // 从相机的（更新的）欧拉角计算Front vector
    void updateCameraVectors(){
        // 计算新的Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif