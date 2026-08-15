#pragma once
#include <SDL3/SDL.h>
#include <GL/glew.h>

class Wave
{
public:
    Wave(class Renderer *renderer);
    ~Wave();

    bool Init();
    void Draw();

private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    GLuint mShader;

    class Renderer *mRenderer;
};