#include <cstdio>
#include <sys/types.h>
#include <vector>

#include "glad/gl.h"
#include "lib/shaders.hpp"
#include "lib/mesh.hpp"

void print_state() {

    system("clear");

    std::cout << "Controles - \n\n";

    std::cout << "[1] -> a = a + " << CHANGE_AMOUNT << "\n";
    std::cout << "[SHIFT + 1] -> a = a - " << CHANGE_AMOUNT << "\n";

    std::cout << "\n[2] -> b = b + " << CHANGE_AMOUNT << "\n";
    std::cout << "[SHIFT + 2] -> b = b - " << CHANGE_AMOUNT << "\n";

    std::cout << "\n[] -> dy = dy + " << CHANGE_AMOUNT << "\n";
    std::cout << "[] -> dy = dy - " << CHANGE_AMOUNT << "\n";
    std::cout << "[] -> dx = dx + " << CHANGE_AMOUNT << "\n";
    std::cout << "[] -> dx = dx - " << CHANGE_AMOUNT << "\n";

    std::cout << "\nVariaveis - \n\n";
    std::cout << "a = " << A << "\n";
    std::cout << "b = " << B << "\n";
    std::cout << "dx = " << DX << "\n";
    std::cout << "dy = " << DY << "\n";

    std::fflush(stdout);
}

void read_initial_state() {

    system("clear");

    std::cout << "A hiperbole vertical possuí a fórmula genérica: -x²/a² + y²/b² = 1\n";
    std::cout << "\nResolvendo para y:\n-> y + dy = √( (x - dx)²/(a² + 1) * b² )\n";

    std::string a, b, dx, dy;

    std::cout << "\nForneca os valores de a e b\n\n";
    std::cout << "a = ";
    std::cin >> a;
    
    std::cout << "b = ";
    std::cin >> b;

    std::cout << "\nForneca o deslocamento inicial (dx e dy)\n\n";
    std::cout << "dx = ";
    std::cin >> dx;
    
    std::cout << "dy = ";
    std::cin >> dy;

    A = stof(a);
    B = stof(b);
    DX = stof(dx);
    DY = stof(dy);
}

int main()
{
    GLFWwindow* window = GLFWStart();

    Shader point_shader = Shader(
        "../shaders/points.vs",
        "../shaders/points.fs"
    );

    Shader grid_shader = Shader(
        "../shaders/grid.vs",
        "../shaders/grid.fs"
    );

    int n = 2;

    uint* VBO = new uint[n];
    uint* VAO = new uint[n];

    glGenVertexArrays(n, VAO);
    glGenBuffers(n, VBO);

    std::vector<float> grid = grid_mesh();

    int grid_n = grid.size();

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, grid_n * sizeof(float), grid.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    read_initial_state();
    print_state();

    // render loop
    glPointSize(3.0f);
    while (!glfwWindowShouldClose(window))
    {
        float tA = A;
        float tB = B;
        float tDX = DX;
        float tDY = DY;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        grid_shader.use();

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_LINES, 0, grid_n / 3.0);

        std::vector<float> points = hyperbole_mesh();

        int points_n = points.size();

        if (points_n > 0) {

            glBindVertexArray(VAO[1]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, points_n * sizeof(float), points.data(), GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            int vcount = ((double)points_n / 3.0);

            if (vcount > 0) {
                point_shader.use();
                point_shader.setFloat("zoom", Zoom);
                glDrawArrays(GL_POINTS, 0, points_n/3);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (tA != A || tB != B || tDX != DX || tDY != DY)
        {
            print_state();
        }
    }

    glDeleteVertexArrays(n, VAO);
    glDeleteBuffers(n, VBO);

    point_shader.del();

    glfwTerminate();

    return 0;
}
