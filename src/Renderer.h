#pragma once
#include <SDL3/SDL.h>

class Renderer
{
public:
    Renderer(class Application *app);

    bool Init();
    void Shutdown();

    void Update(float deltaTime);
    void Draw();

    // ゲッター/セッター
    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }

    SDL_Window *GetWindow() const { return mWindow; }
    SDL_GLContext GetContext() const { return mContext; }

    class GUI *GetGUI() const { return mGUI; }
    class Wave *GetWave() const { return mWave; }

    class Application *GetApplication() const { return mApplication; }

private:
    float mScreenWidth;
    float mScreenHeight;

    SDL_Window *mWindow;
    SDL_GLContext mContext;

    class GUI *mGUI;
    class Wave *mWave;

    class Application *mApplication;
};