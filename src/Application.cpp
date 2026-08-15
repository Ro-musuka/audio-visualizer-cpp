#include "Application.h"

#include <SDL3/SDL.h>

#include "Renderer.h"
#include "GUI.h"
#include "Wave.h"
#include "AudioPlayer.h"
#include "FFT.h"

Application::Application()
    : mIsRunning(true),
      mTicksCount(0),
      mRenderer(nullptr),
      mAudioPlayer(nullptr)
{
}

bool Application::Init(const char *fileName)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDLの初期化に失敗しました: %s", SDL_GetError());
        return false;
    }

    mRenderer = new Renderer(this);
    if (!mRenderer->Init())
    {
        SDL_Log("レンダラーの作成に失敗しました");
        return false;
    }

    mAudioPlayer = new AudioPlayer(this);
    if (!mAudioPlayer->Init(fileName))
    {
        SDL_Log("オーディオシステムが初期化できませんでした");
        return false;
    }

    mFFT = new FFT(this);

    mRenderer->GetGUI()->SetDuration(mAudioPlayer->GetDuration());

    mTicksCount = SDL_GetTicks();

    return true;
}

void Application::Execution()
{
    while (mIsRunning)
    {
        Input();
        Update();
        Output();
    }
}

void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            mIsRunning = false;
            break;
        }
        mRenderer->GetGUI()->ProcessEvent(event);
    }

    const bool *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        if (mAudioPlayer->GetIsPaused() &&
            mAudioPlayer->GetDuration() >= mRenderer->GetGUI()->GetCurrentTime())
        {
            mAudioPlayer->IsPlaying();
        }
    }
    if (keystate[SDL_SCANCODE_S])
    {
        if (!mAudioPlayer->GetIsPaused())
        {
            mAudioPlayer->IsPused();
        }
    }
}

void Application::Update()
{
    while (SDL_GetTicks() <= mTicksCount + 16)
    {
    }

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    mRenderer->Update(deltaTime);
    if (mAudioPlayer->GetDuration() <= mRenderer->GetGUI()->GetCurrentTime())
    {
        mAudioPlayer->IsPused();
    }
    mAudioPlayer->Play();

    mFFT->Update();
}

void Application::Output()
{
    mRenderer->Draw();
}

void Application::Shutdown()
{
    mRenderer->Shutdown();
    delete mRenderer;
    delete mAudioPlayer;
    delete mFFT;
}
