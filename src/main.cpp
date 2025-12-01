// 引用GLAD和GLFW的头文件
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//引入自定义类
#include "my_shader.h"
#include "my_TextureLoader.h"

//帧缓冲大小函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

	// GLFW 初始化和配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// 构建GLFW窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD加载所有函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 线框模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // 这里实现我们的shader项目

    Shader myShader("shader\\Texture.vert","shader\\Texture.frag");
    Texture texture_1("Resource\\Image\\awesomeface.png");
    Texture texture_2("Resource\\Image\\container.jpg");

    // 顶点数组
    //加入纹理的顶点
    float vertices[] = {
        //     ---- 位置 ----      - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    // 初始化代码（只运行一次 (除非你的物体频繁改变)）
    // 定义一个VAO、VBO对象
    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 绑定VAO、VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    // 启用顶点属性 记得关闭哦！！！
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    texture_1.use(0);
    texture_2.use(1);

    // // 初始化矩阵
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f,0.0f,1.0f));
    // trans = glm::scale(trans, glm::vec3(0.5f,0.5f,0.5f));

    myShader.use();
    myShader.setInt("texture_1",0);
    myShader.setInt("texture_2",1);

    // 渲染循环体
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 每帧绘制开始时，以清除上一帧残留内容
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绑定纹理
        texture_1.use(0);
        texture_2.use(1);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f,0.0f,0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));

        myShader.use();
        //变换矩阵传入着色器
        unsigned int transfromLoc = glGetUniformLocation(myShader.ID, "transform");
        glUniformMatrix4fv(transfromLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //解绑
        glBindVertexArray(0);


		// 交换缓冲区和轮询IO事件(键盘鼠标等)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //回收缓冲对象
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);

    // 清理所有的资源并正确地退出应用程序
    glfwTerminate();
    return 0;
}

// 检测特定的键是否被按下，并在每一帧做出处理
void processInput(GLFWwindow* window)
{
    //检查用户是否按下了返回键 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 创建一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// OpenGL渲染窗口的尺寸大小，即视口(Viewport)
    glViewport(0, 0, width, height);
}