#include "Renderer.h"

#include "Application.h"
#include "GUI.h"
#include "Wave.h"

Renderer::Renderer(Application *app)
    : mApplication(app),
      mScreenWidth(480.0f),
      mScreenHeight(270.0f)
{
    mGUI = new GUI(this);
}

bool Renderer::Init()
{
    // ver4.1のopenGLを使用
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    // openGLのコアプロファイルを使用
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // RGBA・深度バッファのサイズを指定
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // ダブルブッファリングの有効化
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //  openGLがハードウェアアクセラレーションを使うように強制する
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // ウィンドウの作成
    mWindow = SDL_CreateWindow("Visualizer",
                               static_cast<int>(mScreenWidth),
                               static_cast<int>(mScreenHeight),
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!mWindow)
    {
        SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
        return false;
    }
    // openGL コンテキストを作成
    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext)
    {
        SDL_Log("OpenGLコンテキストの作成に失敗しました: %s", SDL_GetError());
        return false;
    }

    // GLEWによる設定初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("GLEWの初期化に失敗しました");
        return false;
    }
    glGetError();

    // GLEWによる初期化時に無害なエラーがでるため、
    // エラーをクリアしておく
    glEnable(GL_DEPTH_TEST);

    if (!mGUI->Init())
    {
        SDL_Log("Imguiの初期化に失敗しました");
        return false;
    }

    return true;
}

void Renderer::Update(float deltaTime)
{
    mGUI->Update(deltaTime);
}

void Renderer::Draw()
{
    glClearColor(1.0f, 0.6f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mGUI->Draw();

    SDL_GL_SwapWindow(mWindow);
}

void Renderer::Shutdown()
{
    delete mGUI;

    SDL_GL_DestroyContext(mContext);
    SDL_DestroyWindow(mWindow);
}