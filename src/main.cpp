#define GLFW_DLL 

#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static const struct
{
    float x, y, z;
    float r, g, b;
    float tx, ty;
} vertices[] =
{
    { -0.9f, -0.9f,   0.f,    1.f, 0.f, 0.f,    1.0f,  1.0f},
    { -0.9f,  0.9f,   0.f,    0.f, 1.f, 0.f,    1.0f,  0.0f},
    {  0.9f, -0.9f,   0.f,    0.f, 1.f, 0.f,    0.0f,  1.0f},
    {  0.9f,  0.9f,   0.f,    0.f, 0.f, 1.f,    0.0f,  0.0f}
};

unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
};

const char *vertex_shader_text =
    "#version 330 core\n"
    "in vec3 vPos;\n"
    "in vec3 vCol;\n"
    "in vec2 vTxt;\n"
    "out vec3 color;\n"
    "out vec2 txt;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(vPos, 1.0);\n"
    "   color = vCol;\n"
    "   txt = vTxt;\n"
    "}\0";

const char *fragment_shader_text =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 color;\n"
    "in vec2 txt;\n"
    "uniform sampler2D txtPic;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(txtPic, txt) * vec4(color, 1.0);\n"
    "}\n\0";
    
const unsigned int SCR_WIDTH = 379;
const unsigned int SCR_HEIGHT = 480;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  90.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

static void error_callback(int error, const char* description)
{
    std::cout << stderr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    float cameraSpeed = 25.5 * deltaTime;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 30.0f && fov <= 120.0f)
        fov -= yoffset;
    if (fov <= 30.0f)
        fov = 30.0f;
    if (fov >= 120.0f)
        fov = 120.0f;
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
    
    window = glfwCreateWindow(379, 480, "OGL tst", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


}

int main(void)
{
    // === init ===========================================
    
    GLFWwindow* window;
    
    if (!initGLFW(window))
        exit(EXIT_FAILURE);
    
    GLuint program;
    if (!makeProgramm(program, vertex_shader_text, fragment_shader_text)) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // === vao, vbo, ebo ==================================

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    
    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    
    glBindVertexArray(vertex_array);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    GLint vpos_location = glGetAttribLocation(program, "vPos");
    GLint vcol_location = glGetAttribLocation(program, "vCol");
    GLint vtxt_location = glGetAttribLocation(program, "vTxt");
    
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vpos_location);

    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(vcol_location);
    
    glVertexAttribPointer(vtxt_location, 2, GL_FLOAT, GL_FALSE, 
                          sizeof(vertices[0]), (void*) (sizeof(float) * 6));
    glEnableVertexAttribArray(vtxt_location);
    
    // === texture ========================================
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../resources/gato.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // === draw ===========================================

    glUseProgram(program);
    
    while (!glfwWindowShouldClose(window))
    {
        // === input ==========================================

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // === clear ==========================================        
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // === transform ======================================
        
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(0.0f,  0.0f,  0.0f));
        float angle = 20.0f * 0;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
        
        // === transform ======================================
        
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // === clear ==========================================
    
    
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}
