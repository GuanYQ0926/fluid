#ifndef SCENE_H
#define SCENE_H

#include "glinclude.h"
#include "glslprogram.h"

#include <string>
#include <glm/glm.hpp>

#define viewportWidth 400
#define viewportHeight 500
#define gridWidth (viewportWidth/2)
#define gridHeight (viewportHeight/2)
#define cellSize (1.25f)
#define splatRadius ((float)gridWidth/8.0f)

const float velocityDissipation = 0.9999f;
const float temperatureDissipation = 0.9999f;
const float densityDissipation = 0.9999f;
const glm::ivec2 impulsePosition = glm::ivec2(gridWidth/2, -(int)splatRadius/2);
const float impulseTemperature = 4.0f;
const float impulseDensity = 2.0f;
const float timeStep = 0.025f;
const float ambientTemperature = 0.0f;
const float smokeBuoyancy = 12.0f;
const float smokeWeight = 0.0005f;
const float gradientScale = 1.125f/cellSize;
const int numJacobiIterations = 40;


struct ProgramRec{
    GLSLProgram Advect;
    GLSLProgram Jacobi;
    GLSLProgram SubtractGradient;
    GLSLProgram ComputeDivergence;
    GLSLProgram ApplyImpulse;
    GLSLProgram ApplyBuoyancy;
    GLSLProgram VisualizationTest;
};

struct Surface{
    GLuint fboHandle;
    GLuint textureHandle;
    int numComponents;
};

struct Slab{
    Surface ping;
    Surface pong;
};



class Scene
{
public:
    Scene();
    void initScene();
    void renderScene();

private:
    ProgramRec programs;
    GLSLProgram visualizeProgram;
    GLuint quadVao;
    Slab velocity, density, pressure, temperature;
    Surface divergence, obstacles, hiresObstacles;

    void initPrograms(ProgramRec programs, string shaderNames[]);
    void resetState();
    Slab createSlab(int width, int height, int numComponents);
    Surface createSurface(int width, int height, int numComponents);
    void swapSurfaces(Slab* slab);
    void clearSurface(Surface s, float v);

    void advect(Surface velocity, Surface source, Surface obstacles, Surface dest, float dissipation);
    void jacobi(Surface pressure, Surface divergence, Surface obstacles, Surface dest);
    void subtractGradient(Surface velocity, Surface pressure, Surface obstacles, Surface dest);
    void computeDivergence(Surface velocity, Surface obstacles, Surface dest);
    void applyImpulse(Surface dest, glm::vec2 position, float value);
    void applyBuoyancy(Surface velocity, Surface temperature, Surface density, Surface dest);

    GLuint createQuad();
    void createObstacles(Surface dest, int width, int height);
};

#endif // SCENE_H
