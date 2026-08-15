#include "Wave.h"

#include <algorithm>
#include <cmath>

#include "Application.h"
#include "Renderer.h"
#include "GUI.h"
#include "FFT.h"

static const char *vert_glsl = R"(
#version 410 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in float aHeight;

out float Height;

void main()
{
    Height = aHeight;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

static const char *frag_glsl = R"(
#version 410 core

in float Height;
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.2, 0.8, Height, 1.0);
}
)";

Wave::Wave(Renderer *renderer)
    : mRenderer(renderer),
      mVAO(0),
      mVBO(0),
      mEBO(0), // mEBOを追加
      mShader(0)
{
}

Wave::~Wave()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteProgram(mShader);
}

bool Wave::Init()
{
    // シェーダーコンパイルとリンク
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_glsl, nullptr);
    glCompileShader(vert);

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_glsl, nullptr);
    glCompileShader(frag);

    mShader = glCreateProgram();
    glAttachShader(mShader, vert);
    glAttachShader(mShader, frag);
    glLinkProgram(mShader);

    glDeleteShader(vert);
    glDeleteShader(frag);

    // VAO等を設定
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    const int count = 512;
    std::vector<unsigned int> indices;
    indices.reserve(count * 6);

    for (int i = 0; i < count; ++i)
    {
        unsigned int base = i * 4;
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);

        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // VBOのメモリ領域を確保
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 4 * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // location 1 : height
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return true;
}

void Wave::Draw()
{
    const std::vector<float> &spectrum = mRenderer->GetApplication()->GetFFT()->GetSpectrum();

    float screenWidth = mRenderer->GetScreenWidth();
    float screenHeight = mRenderer->GetScreenHeight();

    int count = 512;
    float width = screenWidth / count;
    float height = mRenderer->GetScreenHeight() / 2;

    std::vector<float> vertices;

    // 頂点を設定
    for (int i = 0; i < count; ++i)
    {
        float x0 = i * width;
        float x1 = (i + 1) * width;

        float y0 = height;
        float magnitude = std::max(spectrum[i], 1e-6f);
        float y1 = height + 20 * log10(magnitude);

        // NDC変換 (-1.0 ~ 1.0)
        float nx0 = (x0 / screenWidth) * 2.0f - 1.0f;
        float nx1 = (x1 / screenWidth) * 2.0f - 1.0f;

        float ny0 = (y0 / screenHeight) * 2.0f - 1.0f;
        float ny1 = (y1 / screenHeight) * 2.0f - 1.0f;

        // x, y, height
        vertices.insert(vertices.end(), {
                                            nx0, ny0, 0.0f, // 左下
                                            nx1, ny0, 0.0f, // 右下
                                            nx1, ny1, 1.0f, // 右上
                                            nx0, ny1, 1.0f  // 左上
                                        });
    }

    // シェーダーの有効化
    glUseProgram(mShader);

    glBindVertexArray(mVAO);

    // 頂点データの更新
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    // 描画
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count * 6), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glUseProgram(0);
}