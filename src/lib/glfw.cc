#include <glm/ext/quaternion_geometric.hpp>
#include <iostream>
#include <cmath>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

const uint SCR_WIDTH = 1400;
const uint SCR_HEIGHT = 1400;

const uint FLOAT_PRECISION = 3;

const float CHANGE_AMOUNT = 0.05;

float A = 1.0f;
float B = 1.0f;

float DX = 0.0f;
float DY = 0.0f;

static float Zoom = 5.0f;

bool first_mouse = false;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool MOUSE_DRAG = false;

glm::vec3 cameraPos(0.0, 0.0, 3.0);
glm::vec3 cameraFront(0.0, 0.0, -1.0);
glm::vec3 worldUp(0.0, 1.0, 0.0);

float cameraSide = 10.0f;

glm::vec3 front = cameraFront;
glm::vec3 side = glm::normalize(glm::cross(front, worldUp));
glm::vec3 up = glm::normalize(glm::cross(side, front));

float round(float x) {
    double pow10 = std::pow(10, FLOAT_PRECISION);
    return (float)((double)std::round(x * pow10) / pow10);
}

void scroll_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float dy = -(float)yoffset * 0.5;
    cameraSide = std::max(cameraSide + dy, 0.3f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (first_mouse) {
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }

    float dx = xpos - lastX;
    float dy = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    if (MOUSE_DRAG) {
        float mult = 0.001 * cameraSide;
        cameraPos += side * -dx * mult;
        cameraPos += up * -dy * mult;
    }
}

void mouse_button(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            MOUSE_DRAG = true;
        }
        if (action == GLFW_RELEASE) {
            MOUSE_DRAG = false;
        }
    }
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button);

    // Start glad
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    return window;
}
