#include <iostream>
#include <sstream>
#include <string>

#include "glinclude.h"
#include "scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



int main()
{
    int frames = 0;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(viewportWidth, viewportHeight,
                                          "fluid", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    Scene* scene = new Scene();
    scene->initScene();

    GLfloat lastFrame = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat currentFrame = glfwGetTime();
        if(currentFrame-lastFrame>=1.0)
        {
            string title;
            std::stringstream sfps;
            sfps << double(frames);//double(currentFrame-lastFrame);
            sfps >> title;
            lastFrame =glfwGetTime();//+= 1.0f;
            glfwSetWindowTitle(window, title.c_str());
            frames = 0;
        };

        scene->renderScene();

        glfwSwapBuffers(window);
        frames++;
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

