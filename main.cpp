#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "Buff/VertexBuffer.h"
#include "Buff/VertexArray.h"
#include "Buff/ElementBuffer.h"
#include "Buff/Texture2D.h"
#include "ErrorHandling/error_controlling.h"
#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define WINDOW_W 1280
#define WINDOW_H 800

/* Camera */

Camera cam(glm::vec3(-5.0f,0,0));

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float lastX = 400, lastY = 300;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam.add_yaw(xoffset);
    cam.add_pitch(yoffset);
}

float time_glfw = 0;

double getPassed()
{
    float passed = glfwGetTime() - time_glfw;
    time_glfw = glfwGetTime();
    return passed;
}

int main() {
    Err::restart_gl_log();
    Err::gl_log("Hello sir, thats ur glfw version : "+std::string(glfwGetVersionString()));
    glfwSetErrorCallback(Err::glfw_error_callback);

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    /* Screen Resolution Settings */
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* vmode = glfwGetVideoMode(mon);

    /* Window Init */
    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Roargh", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    /* Cursor dont fuckin move */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /* Setting mouse_callback as CursorPosCallback */
    glfwSetCursorPosCallback(window, mouse_callback);
    /* Window size callback */
    glfwSetWindowSizeCallback(window, glfw_window_size_callback);
    /* Logging Params Window Params*/
    Err::log_gl_params();

    // start GLEW  extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // CubeVertexXYZ + NormalXYZ
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    /* Object 1 */
    VertexBuffer vb(vertices, sizeof(vertices));
    /* Pos Buffer */
                    //Read 3 float and skip 6 float and read again. Start from 0
    VertexArray va(vb, 3, 6 * sizeof(float), (void*)0);
    /* Normal Buffer */
                    //Read 3 float and skip 6 float and read again. Start from 3
    va.addBuffer(vb, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    /* Object Shader */
    Program lightShader(Program::createProgram("Shaders/shader.glsl"));
    Err::print_all(lightShader.getId());

    /* Light Shader */                           //Just white
    Program objectShader(Program::createProgram("Shaders/light_shader.glsl"));
    Err::print_all(objectShader.getId());

    /* Positions */
    glm::vec3 lightPos(5.0f, -5, -10.0f), 
              objectPos(0.0f, 0.0f, 0.0f);

    glm::vec3 light_color(1.0f, 1.0f, 1.0f);

    /* Projection matrix */
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 640.0f / 360.0f, 0.1f, 100.0f);

    /* Object Model Matrix */
    glm::mat4 object_model = glm::scale(glm::translate(glm::mat4(1.0f), objectPos),glm::vec3(1.0f));
    /* Lightning Model Matrix */
    glm::mat4 lightning_model = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(1.0f));

    /* Setting Uniforms */

    lightShader.use();
    lightShader.setMat4("model", lightning_model); lightShader.setMat4("projection", projection);
    
    /* Setting Uniforms */
    objectShader.use();
    objectShader.setMat4("model", object_model); objectShader.setMat4("projection", projection);
    objectShader.setVec3("lightColor", light_color);
    objectShader.setVec3("lightPos", lightPos);
    objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        double passed = getPassed(); // Getting passed time between 2 frames.
        cam.calc_passed(passed); // Giving info to camera system for moving camera
        /* Light movement */
        lightPos.x = 20.0f * sin(glfwGetTime()/2);
        lightPos.y = -20.0f * sin(glfwGetTime()/5);
        lightPos.z = 20.0f * cos(glfwGetTime()/3);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // with GL_DEPTH_TEST

        lightShader.use();
        lightShader.setMat4("view", cam.getView());
        /* For drawing light, translating to lightPos */
        lightning_model = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(1.0f));
        lightShader.setMat4("model", lightning_model);

        va.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        objectShader.use(); // light and view position for light calculations
        objectShader.setMat4("view", cam.getView());
        objectShader.setVec3("lightPos", lightPos);
        objectShader.setVec3("viewPos", cam.get_pos());

        // Drawing cubes
        for (int x = 0; x < 10; x++) for (int y = 0; y < 5; y++) for (int z = 0; z < 10; z++)
        {
            object_model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(x*2,y*2,z*2)), glm::vec3(1.0f));
            objectShader.setMat4("model", object_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwPollEvents(); 
        glfwSwapBuffers(window);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, 1);
        // Camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.move_forward();
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.move_backward();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.move_right();
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.move_left();
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}