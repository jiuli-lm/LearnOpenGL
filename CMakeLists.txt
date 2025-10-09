#最小需求版本号 用一些特性需要去指定版本
cmake_minimum_required(VERSION 3.15)
project(
    #名称 必填
    OPENGL
    #使用语言 描述 项目版本 可选
    LANGUAGES CXX C
    DESCRIPTION "OpenGL learn"
    VERSION 0.1.0
)
set(CMAKE_CXX_STANDARD 17)
# 用于生成 compile_commands.json
set(EXPORT_COMPILE_COMMANDS ON)

# 源文件
# GLOB 表示“全局通配符匹配” 小项目可以使用 大项目不推荐
file(GLOB SRC src/*.cpp src/*.c)
#file(COPY ${CMAKE_SOURCE_DIR}/shader DESTINATION ${CMAKE_BINARY_DIR})

add_compile_options(/utf-8)
add_executable(${PROJECT_NAME} ${SRC})

# 编译选项：强制 MSVC 按 UTF-8 编译
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# 头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdFiles/include
    ${CMAKE_SOURCE_DIR}/myClass
)

# 库目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdFiles/lib
)

# 链接 GLFW 和 OpenGL
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw3
    opengl32
    user32
    gdi32
    shell32
)

# 在生成exe后 把shader文件复制到执行文件同级目录中
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader
)