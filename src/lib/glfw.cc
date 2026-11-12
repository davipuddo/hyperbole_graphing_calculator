#include <iostream>
#include <cmath>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 800;

const uint FLOAT_PRECISION = 3;

const float CHANGE_AMOUNT = 0.05;

float A = 1.0f;
float B = 1.0f;

float DX = 0.0f;
float DY = 0.0f;

static float Zoom = 5.0f;

glm::vec3 cameraPos(0.0, 0.0, 3.0);
glm::vec3 cameraFront(0.0, 0.0, -1.0);
glm::vec3 worldUp(0.0, 1.0, 0.0);

float round(float x) {
    double pow10 = std::pow(10, FLOAT_PRECISION);
    return (float)((double)std::round(x * pow10) / pow10);
}

void processInput(GLFWwindow *window)
{
}

void scroll_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float dy = (float)yoffset * 0.5;
    float dx = -(float)xoffset * 0.5;

    cameraFront += glm::vec3(dx, dy, 0.0f);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
        switch (key) {

            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, true);
            break;

            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
            break;

            case GLFW_KEY_1: 
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                    A -= CHANGE_AMOUNT;
                } else {
                    A += CHANGE_AMOUNT;
                }
                A = round(A);
                std::cout << "a = " << A << std::endl;
            break;

            case GLFW_KEY_2:
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                    B -= CHANGE_AMOUNT;
                } else {
                    B += CHANGE_AMOUNT;
                }
                B = round(B);
                std::cout << "b = " << B << std::endl;
            break;

            case GLFW_KEY_UP:
                DY += CHANGE_AMOUNT;
                DY = round(DY);
            break;

            case GLFW_KEY_DOWN:
                DY -= CHANGE_AMOUNT;
                DY = round(DY);
            break;

            case GLFW_KEY_RIGHT:
                DX += CHANGE_AMOUNT;
                DX = round(DX);
            break;

            case GLFW_KEY_LEFT:
                DX -= CHANGE_AMOUNT;
                DX = round(DX);
            break;
        };
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* GLFWStart() {

    // Initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "gstuff", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_wheel_callback);
    glfwSetKeyCallback(window, keyboard_callback);

    // Start glad
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    return window;
}
