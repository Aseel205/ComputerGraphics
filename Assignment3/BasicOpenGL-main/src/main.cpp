#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <smallCube.h>
#include <RubiksCube.h>
#include <iostream>
#include <globals.h>

/* Window size */
const unsigned int width = 800;
const unsigned int height = 800;
// const float FOVdegree = 45.0f;  // Field Of View Angle
const float FOVdegree = 45.0f;  // Field Of View Angle
const float near = 0.1f;
const float far = 100.0f;
RubiksCube rubiksCube;


float vertices[] = {
    // positions           // colors           // texCoords
    // Front face
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Top-left

    // Back face
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f, // Top-left

    // Left face
    -0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left
    -0.5f, -0.5f,  0.5f,   0.5f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.5f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,   0.5f, 0.0f, 1.0f,   0.0f, 1.0f, // Top-left

    // Right face
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.5f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,   0.5f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.5f,   1.0f, 1.0f, // Top-right
     0.5f,  0.5f, -0.5f,   0.0f, 0.5f, 1.0f,   0.0f, 1.0f, // Top-left

    // Top face
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Top-left

    // Bottom face
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
    -0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f  // Top-left
};

/* Indices for cube faces */
unsigned int indices[] = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Left face
    8, 9, 10, 10, 11, 8,
    // Right face
    12, 13, 14, 14, 15, 12,
    // Top face
    16, 17, 18, 18, 19, 16,
    // Bottom face
    20, 21, 22, 22, 23, 20
};

int main(int argc, char* argv[]) {
    GLFWwindow* window;
    const unsigned int width = 800, height = 600;
    const float FOVdegree = 45.0f, near = 0.1f, far = 100.0f;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Set OpenGL version to 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a window */
    window = glfwCreateWindow(width, height, "OpenGL Rubik's Cube", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);


    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    {
        /* Enable depth testing */
        GLCall(glEnable(GL_DEPTH_TEST));
    
        /* Set up buffers and shaders */
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indices, sizeof(indices));

        VertexBufferLayout layout;
        layout.Push<float>(3); // positions
        layout.Push<float>(3); // colors
        layout.Push<float>(2); // texCoords
        va.AddBuffer(vb, layout);

        Texture texture("res/textures/plane.png");
        texture.Bind();

        Shader shader("res/shaders/basic.shader");
        shader.Bind();

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        /* Initialize the camera */
        Camera camera(width, height);
        camera.SetPerspective(FOVdegree, near, far);
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f)); // Move camera back
        camera.EnableInputs(window);


        while (!glfwWindowShouldClose(window)) {
            GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Set background to dark gray
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            glm::mat4 view = camera.GetViewMatrix();  // Camera view matrix
            glm::mat4 proj = camera.GetProjectionMatrix();  // Camera projection matrix

            // Render Rubik's Cube using the RubiksCube class
            rubiksCube.render(shader, va, ib, proj, view);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

 
