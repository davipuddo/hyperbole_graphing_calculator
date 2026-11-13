#include <vector>

#include "glad/gl.h"
#include "lib/shaders.hpp"
#include "lib/mesh.hpp"

using namespace glm;
using namespace std;

void print_state() {

    system("clear");

    cout << "Controles - \n\n";

    cout << "[1] -> a = a + " << CHANGE_AMOUNT << "\n";
    cout << "[SHIFT + 1] -> a = a - " << CHANGE_AMOUNT << "\n";

    cout << "\n[2] -> b = b + " << CHANGE_AMOUNT << "\n";
    cout << "[SHIFT + 2] -> b = b - " << CHANGE_AMOUNT << "\n";

    cout << "\n[] -> dy = dy + " << CHANGE_AMOUNT << "\n";
    cout << "[] -> dy = dy - " << CHANGE_AMOUNT << "\n";
    cout << "[] -> dx = dx + " << CHANGE_AMOUNT << "\n";
    cout << "[] -> dx = dx - " << CHANGE_AMOUNT << "\n";

    cout << "\nVariaveis - \n\n";
    cout << "a = " << A << "\n";
    cout << "b = " << B << "\n";
    cout << "dx = " << DX << "\n";
    cout << "dy = " << DY << "\n";

    fflush(stdout);
}

void read_initial_state() {

    system("clear");

    cout << "A hiperbole vertical possuí a fórmula genérica: -x²/a² + y²/b² = 1\n";
    cout << "\nResolvendo para y:\n-> y - dy = √( ((x - dx)²/a² + 1) * b² )\n";

    string a, b, dx, dy;

    cout << "\nForneca os valores de a e b\n\n";
    cout << "a = ";
    cin >> a;
    
    cout << "b = ";
    cin >> b;

    cout << "\nForneca o deslocamento inicial (dx e dy)\n\n";
    cout << "dx = ";
    cin >> dx;
    
    cout << "dy = ";
    cin >> dy;

    A = stof(a);
    B = stof(b);
    DX = stof(dx);
    DY = stof(dy);
}

int main()
{
    // Create window
    GLFWwindow* window = GLFWStart();

    // Create & compile shaders
    Shader point_shader = Shader(
        "../shaders/points.vs",
        "../shaders/points.fs"
    );

    Shader grid_shader = Shader(
        "../shaders/grid.vs",
        "../shaders/grid.fs"
    );

    // Define VBO's and VAO's
    int n = 2;

    uint* VBO = new uint[n];
    uint* VAO = new uint[n];

    glGenVertexArrays(n, VAO);
    glGenBuffers(n, VBO);

    // Create grid's mesh, and send it to the GPU
    vector<float> grid = grid_mesh();

    int grid_n = grid.size();

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, grid_n * sizeof(float), grid.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    read_initial_state();

    // Define frustum
    float near = 0.1f;
    float far = 100.0f;

    // Create hyperbole's mesh
    vector<float> points = hyperbole_mesh();

    glPointSize(3.0f);
    glLineWidth(3.0f);

    print_state();

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Temporary values to avoid calculations
        float prevA = A;
        float prevB = B;
        float prevDX = DX;
        float prevDY = DY;
        float prevSide = cameraSide;

        // Create current view matrix
        mat4 view = lookAt(cameraPos, cameraPos + cameraFront, worldUp);

        // Make grid strech into the window's borders
        mat4 model = mat4(1.0);
        model = scale(model, vec3((float)SCR_HEIGHT));

        // Create projection matrix | left  right  bottom  top  --  --
        mat4 proj = ortho(-cameraSide, cameraSide, -cameraSide, cameraSide, near, far);

        // Attach shader
        grid_shader.use();

        // Send uniforms
        grid_shader.setMat4("model", model);
        grid_shader.setMat4("view", view);
        grid_shader.setMat4("proj", proj);

        // Render grid
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_LINES, 0, grid_n / 3.0);

        int points_n = points.size();

        if (points_n > 0) {

            // Send point's mesh to the GPU
            glBindVertexArray(VAO[1]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, points_n * sizeof(float), points.data(), GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3.0 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            int vcount = ((double)points_n / 3.0);

            // Avoid render if no points exist
            if (vcount > 0) {

                // Reset model matrix
                model = mat4(1.0f);

                // Attach shader
                point_shader.use();

                // Send uniforms
                point_shader.setMat4("model", model);
                point_shader.setMat4("view", view);
                point_shader.setMat4("proj", proj);

                // Render points
                glDrawArrays(GL_POINTS, 0, points_n / 3.0f);
            }
        }

        // Display buffer + execute events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update points & print, on new inputs
        if (prevA != A || prevB != B || prevDX != DX || prevDY != DY || prevSide != cameraSide)
        {
            points = hyperbole_mesh();
            print_state();
        }
    }

    // Cleanup

    glDeleteVertexArrays(n, VAO);
    glDeleteBuffers(n, VBO);

    point_shader.del();
    grid_shader.del();

    glfwTerminate();

    return 0;
}
