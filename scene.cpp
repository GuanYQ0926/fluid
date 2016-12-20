#include "scene.h"

#include <stdio.h>
#include <math.h>



Scene::Scene()
{
    //Advect
    if(!programs.Advect.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                              GLSLShader::VERTEX))
    {
        printf("Advect::Vertex shader failed to compile!\n%s",
               programs.Advect.log().c_str());
        exit(1);
    }
    if(!programs.Advect.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/advect.fs",
                                              GLSLShader::FRAGMENT))
    {
        printf("Advect::Fragment shader failed to compile!\n%s",
               programs.Advect.log().c_str());
        exit(1);
    }
    if(!programs.Advect.link())
    {
        printf("Advect::Shader program failed to link!\n");
        exit(1);
    }
    //Jacobi
    if(!programs.Jacobi.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                              GLSLShader::VERTEX))
    {
        printf("Jacobi::Vertex shader failed to compile!\n%s",
               programs.Jacobi.log().c_str());
        exit(1);
    }
    if(!programs.Jacobi.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/jacobi.fs",
                                              GLSLShader::FRAGMENT))
    {
        printf("Jacobi::Fragment shader failed to compile!\n%s",
               programs.Jacobi.log().c_str());
        exit(1);
    }
    if(!programs.Jacobi.link())
    {
        printf("Jacobi::Shader progrom failed to link!\n");
        exit(1);
    }
    //SubtractGradient
    if(!programs.SubtractGradient.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                                        GLSLShader::VERTEX))
    {
        printf("SubtractGradient::Vertex shader failed to compile!\n%s",
               programs.SubtractGradient.log().c_str());
        exit(1);
    }
    if(!programs.SubtractGradient.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/subtractGradient.fs",
                                                        GLSLShader::FRAGMENT))
    {
        printf("SubtractGradient::Fragment shader failed to compile!\n%s",
               programs.SubtractGradient.log().c_str());
        exit(1);
    }
    if(!programs.SubtractGradient.link())
    {
        printf("SubtractGradient::Shader program failed to link!\n");
        exit(1);
    }
    //ComputeDivergence
    if(!programs.ComputeDivergence.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                                         GLSLShader::VERTEX))
    {
        printf("ComputeDivergence::Vertex shader failed to compile!\n%s",
               programs.ComputeDivergence.log().c_str());
        exit(1);
    }
    if(!programs.ComputeDivergence.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/computeDivergence.fs",
                                                         GLSLShader::FRAGMENT))
    {
        printf("ComputeDivergence::Fragment shader failed to compile!\n%s",
               programs.ComputeDivergence.log().c_str());
        exit(1);
    }
    if(!programs.ComputeDivergence.link())
    {
        printf("ComputeDivergence::Shader program failed to link!\n");
        exit(1);
    }
    //ApplyImpulse
    if(!programs.ApplyImpulse.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                                    GLSLShader::VERTEX))
    {
        printf("ApplyImpulse::Vertex shader failed to compile!\n%s",
               programs.ApplyImpulse.log().c_str());
        exit(1);
    }
    if(!programs.ApplyImpulse.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/splat.fs",
                                                    GLSLShader::FRAGMENT))
    {
        printf("ApplyImpulse::Fragment shader failed to compile!\n%s",
               programs.ApplyImpulse.log().c_str());
        exit(1);
    }
    if(!programs.ApplyImpulse.link())
    {
        printf("ApplyImpulse::Shader program failed to link!\n");
        exit(1);
    }
    //ApplyBuoyancy
    if(!programs.ApplyBuoyancy.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                                     GLSLShader::VERTEX))
    {
        printf("ApplyBuoyancy::Vertex shader failed to compile!\n%s",
               programs.ApplyBuoyancy.log().c_str());
        exit(1);
    }
    if(!programs.ApplyBuoyancy.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/buoyancy.fs",
                                                     GLSLShader::FRAGMENT))
    {
        printf("ApplyBuoyancy::Fragment shader failed to compile!\n%s",
               programs.ApplyBuoyancy.log().c_str());
        exit(1);
    }
    if(!programs.ApplyBuoyancy.link())
    {
        printf("ApplyBuoyancy::Shader program failed to link!\n");
        exit(1);
    }
    //Visualization test
    if(!programs.VisualizationTest.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                                         GLSLShader::VERTEX))
    {
        printf("Visualization_TEST::Vertex shader failed to compile!\n%s",
               programs.VisualizationTest.log().c_str());
        exit(1);
    }
    if(!programs.VisualizationTest.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/visualize.fs",
                                                         GLSLShader::FRAGMENT))
    {
        printf("Visualization_TEST::Fragment shader failed to compile!\n%s",
               programs.VisualizationTest.log().c_str());
        exit(1);
    }
    if(!programs.VisualizationTest.link())
    {
        printf("Visualization_TEST::Shaders failed to compile!\n%s",
              programs.VisualizationTest.log().c_str());
        exit(1);
    }
}

void Scene::initScene()
{
    int w = gridWidth;
    int h = gridHeight;
    velocity = createSlab(w, h, 2);
    density = createSlab(w, h, 1);
    pressure = createSlab(w, h, 1);
    temperature = createSlab(w, h, 1);
    divergence = createSurface(w, h, 3);

    /*
    if(!visualizeProgram.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                               GLSLShader::VERTEX))
    {
        printf("VisualizeProgram::Vertex shader failed to compile!\n%s",
               visualizeProgram.log().c_str());
        exit(1);
    }
    if(!visualizeProgram.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/visualize.fs",
                                               GLSLShader::FRAGMENT))
    {
        printf("VisualizeProgram::Fragment shader failed to compile!\n%s",
               visualizeProgram.log().c_str());
        exit(1);
    }
    if(!visualizeProgram.link())
    {
        printf("VisualizeProgram::Shader program failed to link!\n");
        exit(1);
    }
    visualizeProgram.use();
    */
    //programs.VisualizationTest.use();

    obstacles = createSurface(w, h, 3);
    createObstacles(obstacles, w, h);

    w = viewportWidth * 2;
    h = viewportHeight * 2;
    hiresObstacles = createSurface(w, h, 1);
    createObstacles(hiresObstacles, w, h);

    quadVao = createQuad();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    clearSurface(temperature.ping, ambientTemperature);
}

void Scene::renderScene()
{
    glViewport(0, 0, gridWidth, gridHeight);
    //update
    advect(velocity.ping, velocity.ping, obstacles,
           velocity.pong, velocityDissipation);
    swapSurfaces(&velocity);

    advect(velocity.ping, temperature.ping, obstacles,
           temperature.pong, temperatureDissipation);
    swapSurfaces(&temperature);

    advect(velocity.ping, density.ping, obstacles,
           density.pong, densityDissipation);
    swapSurfaces(&density);

    applyBuoyancy(velocity.ping, temperature.ping, density.ping,
                  velocity.pong);
    swapSurfaces(&velocity);

    applyImpulse(temperature.ping, impulsePosition, impulseTemperature);
    applyImpulse(density.ping, impulsePosition, impulseDensity);

    computeDivergence(velocity.ping, obstacles, divergence);
    clearSurface(pressure.ping, 0);
    for(int i=0;i<numJacobiIterations;i++)
    {
        jacobi(pressure.ping, divergence, obstacles, pressure.pong);
        swapSurfaces(&pressure);
    }

    subtractGradient(velocity.ping, pressure.ping, obstacles, velocity.pong);
    swapSurfaces(&velocity);
    /*
    //render
    visualizeProgram.use();
    glEnable(GL_BLEND);

    glViewport(0, 0, viewportWidth, viewportHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //draw ink
    glBindTexture(GL_TEXTURE_2D, density.ping.textureHandle);
    visualizeProgram.setUniform("FillColor", glm::vec3(1));
    visualizeProgram.setUniform("Scale", glm::vec2(1.0f/viewportWidth, 1.0f/viewportHeight));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //draw obstacles
    glBindTexture(GL_TEXTURE_2D, hiresObstacles.textureHandle);
    visualizeProgram.setUniform("FillColor", glm::vec3(0.125f, 0.45f, 0.45f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);*/

    //render
    programs.VisualizationTest.use();
    glEnable(GL_BLEND);

    glViewport(0, 0, viewportWidth, viewportHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //draw ink
    glBindTexture(GL_TEXTURE_2D, density.ping.textureHandle);
    programs.VisualizationTest.setUniform("FillColor", glm::vec3(1));
    programs.VisualizationTest.setUniform("Scale", glm::vec2(1.0f/viewportWidth, 1.0f/viewportHeight));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //draw obstacles
    glBindTexture(GL_TEXTURE_2D, hiresObstacles.textureHandle);
    programs.VisualizationTest.setUniform("FillColor", glm::vec3(0.125f, 0.45f, 0.45f));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);
}

Slab Scene::createSlab(int width, int height, int numComponents)
{
    Slab slab;
    slab.ping = createSurface(width, height, numComponents);
    slab.pong = createSurface(width, height, numComponents);
    return slab;
}

Surface Scene::createSurface(int width, int height, int numComponents)
{
    GLuint fboHandle;
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch(numComponents)
    {
    case 1: glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_HALF_FLOAT, 0);
        break;
    case 2: glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_HALF_FLOAT, 0);
        break;
    case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_HALF_FLOAT, 0);
        break;
    case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, 0);
        break;
    default:
        printf("CreateSurface::Slab format error.\n");
        exit(1);
    }

    //GLuint colorbuffer;
    //glGenRenderbuffers(1, &colorbuffer);
    //glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    {
        printf("ERROR::FRAMEBUFFER::Framebuffer is not complete!\n");
    }
    Surface surface = {fboHandle, textureHandle, numComponents};

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return surface;
}

GLuint Scene::createQuad()
{
    int positions[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1
    };
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 0,
                          ((GLubyte*)NULL + (0)));
    return vao;
}

void Scene::createObstacles(Surface dest, int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLSLProgram obstaclesProgram;
    if(!obstaclesProgram.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/vertex.vs",
                                               GLSLShader::VERTEX))
    {
        printf("CreateObstacles::Vertex shader failed to compile!\n%s",
               obstaclesProgram.log().c_str());
        exit(1);
    }
    if(!obstaclesProgram.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/fluid/fluid/shaders/fill.fs",
                                               GLSLShader::FRAGMENT))
    {
        printf("CreateObstacles::Fragment shader failed to compile!\n%s",
               obstaclesProgram.log().c_str());
        exit(1);
    }
    if(!obstaclesProgram.link())
    {
        printf("CreateObstacles::Shader program failed to link!\n");
        exit(1);
    }
    obstaclesProgram.use();

    //draw border
    const float t = 0.9999f;
    float positions1[] = {
        -t, -t,
         t, -t,
         t,  t,
        -t,  t,
        -t, -t
    };
    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions1), positions1,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,
                          ((GLubyte*)NULL + (0)));
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    glDeleteBuffers(1, &vbo1);
    //draw circles
    const int slices = 64;
    float positions[slices*2*3];
    float twopi = 8*atan(1.0f);
    float theta = 0;
    float dtheta = twopi / (float) (slices - 1);
    float* pPositions = &positions[0];
    for (int i = 0; i < slices; i++) {
        *pPositions++ = 0;
        *pPositions++ = 0;

        *pPositions++ = 0.25f * cos(theta) * height / width;
        *pPositions++ = 0.25f * sin(theta);
        theta += dtheta;

        *pPositions++ = 0.25f * cos(theta) * height / width;
        *pPositions++ = 0.25f * sin(theta);
    }
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,
                          ((GLubyte*)NULL + (0)));
    glDrawArrays(GL_TRIANGLES, 0, slices*3);
    glDeleteBuffers(1, &vbo);

    glDeleteProgram(obstaclesProgram.getHandle());
    glDeleteVertexArrays(1, &vao);
}

void Scene::resetState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
}

void Scene::swapSurfaces(Slab* slab)
{
    Surface temp = slab->ping;
    slab->ping = slab->pong;
    slab->pong = temp;
}

void Scene::clearSurface(Surface s, float v)
{
    glBindFramebuffer(GL_FRAMEBUFFER, s.fboHandle);
    glClearColor(v, v, v, v);
    glClear(GL_COLOR_BUFFER_BIT);
}



void Scene::advect(Surface velocity, Surface source, Surface obstacles,
                   Surface dest, float dissipation)
{
    programs.Advect.use();
    programs.Advect.setUniform("InverseSize", vec2(1.0f/gridWidth,
                                                   1.0f/gridHeight));
    programs.Advect.setUniform("TimeStep", timeStep);
    programs.Advect.setUniform("Dissipation", dissipation);
    programs.Advect.setUniform("SourceTexture", 1);
    programs.Advect.setUniform("Obstacles", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.textureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, source.textureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obstacles.textureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    resetState();
}

void Scene::applyBuoyancy(Surface velocity, Surface temperature,
                          Surface density, Surface dest)
{
    programs.ApplyBuoyancy.use();
    programs.ApplyBuoyancy.setUniform("Temperature", 1);
    programs.ApplyBuoyancy.setUniform("Density", 2);
    programs.ApplyBuoyancy.setUniform("AmbientTemperature", ambientTemperature);
    programs.ApplyBuoyancy.setUniform("TimeStep", timeStep);
    programs.ApplyBuoyancy.setUniform("Sigma", smokeBuoyancy);
    programs.ApplyBuoyancy.setUniform("Kappa", smokeWeight);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.textureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, temperature.textureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, density.textureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    resetState();
}

void Scene::applyImpulse(Surface dest, glm::vec2 position, float value)
{
    programs.ApplyImpulse.use();
    programs.ApplyImpulse.setUniform("Point", position);
    programs.ApplyImpulse.setUniform("Radius", splatRadius);
    programs.ApplyImpulse.setUniform("FillColor", glm::vec3(value));

    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    resetState();
}

void Scene::computeDivergence(Surface velocity, Surface obstacles,
                              Surface dest)
{
    programs.ComputeDivergence.use();
    programs.ComputeDivergence.setUniform("HalfInverseCellSize",
                                          0.5f/cellSize);
    programs.ComputeDivergence.setUniform("Obstacles", 1);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.textureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obstacles.textureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    resetState();
}

void Scene::jacobi(Surface pressure, Surface divergence,
                   Surface obstacles, Surface dest)
{
    programs.Jacobi.use();
    programs.Jacobi.setUniform("Alpha", -cellSize*cellSize);
    programs.Jacobi.setUniform("InverseBeta", 0.25f);
    programs.Jacobi.setUniform("Divergence", 1);
    programs.Jacobi.setUniform("Obstacles", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pressure.textureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, divergence.textureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obstacles.textureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    resetState();
}

void Scene::subtractGradient(Surface velocity, Surface pressure,
                             Surface obstacles, Surface dest)
{
    programs.SubtractGradient.use();
    programs.SubtractGradient.setUniform("GradientScale", gradientScale);
    programs.SubtractGradient.setUniform("Pressure", 1);
    programs.SubtractGradient.setUniform("Obstacles", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.textureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pressure.textureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obstacles.textureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    resetState();
}
