#define GLFW_DLL 

#include "glad.h"
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <iostream>
#include <string>

static const struct
{
    float x, y, z;
    float r, g, b;
} vertices[] =
{
    { -0.1f,  0.1f, -0.9f, 1.f, 0.f, 0.f },
    {  0.8f,  0.2f, -0.9f, 0.f, 1.f, 0.f },
    {   0.f, -1.0f, -0.9f, 0.f, 0.f, 1.f },

    { -0.6f,  0.4f,   0.f, 1.f, 0.f, 0.f },
    {  0.6f,  0.4f,   0.f, 0.f, 1.f, 0.f },
    {   0.f, -0.6f,   0.f, 0.f, 0.f, 1.f },    
};

const char *vertex_shader_text =
    "#version 330 core\n"
    "attribute vec3 vPos;\n"
    "attribute vec3 vCol;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(vPos, 1.0);\n"
    "   color = vCol;\n"
    "}\0";

const char *fragment_shader_text =
    "#version 330 core\n"
    "varying vec3 color;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = vec4(color, 1.0f);\n"
    "}\n\0";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

bool initGLFW(GLFWwindow* &window)
{
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        return false;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // gladLoadGL();
    // glfwSwapInterval(1);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;
    }
    
    return true;
}

int load_shader(
    GLuint &shader,
    GLenum shader_type,
    const char* shader_text,
    std::string error_str)
{
    int  success;
    char infoLog[512];
    
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_text, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << error_str << infoLog << std::endl;
    }
    
    return success;
}

bool load_vertex_shader(GLuint &shader, const char* shader_text)
{
    int success = load_shader(
        shader,
        GL_VERTEX_SHADER,
        shader_text,
        "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        
    return success ? true : false;
}

bool load_fragment_shader(GLuint &shader, const char* shader_text)
{
    int success = load_shader(
        shader,
        GL_FRAGMENT_SHADER,
        shader_text,
        "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        
    return success ? true : false;
}

bool load_program(GLuint &program, GLuint vertex_shader, GLuint fragment_shader)
{
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int  success;
    char infoLog[512];
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    return success ? true : false;
}

bool makeProgramm(
    GLuint &program,
    const char* vertex_shader_text,
    const char* fragment_shader_text)
{
    GLuint vertex_shader, fragment_shader;
    
    if (!load_vertex_shader(vertex_shader, vertex_shader_text))
        return false;
    
    if (!load_fragment_shader(fragment_shader, fragment_shader_text))
        return false;

    if (!load_program(program, vertex_shader, fragment_shader))
        return false;
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader); 
    
    return true;
}

int main(void)
{
    GLFWwindow* window;
    
    if (!initGLFW(window))
        exit(EXIT_FAILURE);
    
    GLuint program;
    if (!makeProgramm(program, vertex_shader_text, fragment_shader_text)) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    
    glBindVertexArray(vertex_array);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    
    
    GLint vpos_location = glGetAttribLocation(program, "vPos");
    GLint vcol_location = glGetAttribLocation(program, "vCol");
    
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vpos_location);

    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(vcol_location);

    glUseProgram(program);
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}
