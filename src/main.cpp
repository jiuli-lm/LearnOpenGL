// 引用GLAD和GLFW的头文件
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    Texture myTexture("Resource\\Image\\awesomeface.png");

    // 顶点数组 这里是三角形的三个顶点坐标
    float vertices_1[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    float vertices_2[] = {
        // 第一个三角形
        -0.5f,  0.5f, 0.0f,  // 左上
        -0.5f, -0.5f, 0.0f,  // 左下
         0.5f, -0.5f, 0.0f,  // 右下
        // 第二个三角形
        -0.5f,  0.5f, 0.0f,  // 左上
         0.5f, -0.5f, 0.0f,  // 右下
         0.5f,  0.5f, 0.0f   // 右上
    };
    
    //加入纹理的顶点
    float vertices_4[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    // 使用 EBO 
    float vertices_3[] = {
        -0.5f,  0.5f, 0.0f,  // 左上
        -0.5f, -0.5f, 0.0f,  // 左下
         0.5f, -0.5f, 0.0f,  // 右下
         0.5f,  0.5f, 0.0f   // 右上
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_4), vertices_4, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // 设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    // 启用顶点属性 记得关闭哦！！！
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    myTexture.use();
    myShader.use();
    glUniform1i(glGetUniformLocation(myShader.ID,"ourTexture"),0);
    // myShader.setInt("ourTexture",1);

    // 渲染循环体
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 每帧绘制开始时，以清除上一帧残留内容
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制物体
        myTexture.use();
        myShader.use();

        
        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) * 0.5f + 0.5f;

        // 计算uniform位置值
        // int offsetLoc = glGetUniformLocation(myShader.ID, "myColor");
        // glUniform4f(offsetLoc, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES,0,6);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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