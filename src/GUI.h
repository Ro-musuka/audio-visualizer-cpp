#pragma once

#include <SDL3/SDL.h>

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_sdl3.h"
#include "../external/imgui/imgui_impl_opengl3.h"

class GUI
{
public:
    GUI(class Renderer *renderer);
    ~GUI();

    // AudioPlayer初期化後
    bool Init();

    void ProcessEvent(SDL_Event event);
    void Input();
    void Update(float deltaTime);
    void Draw();

    // ゲッター/セッター
    float GetThickness() const { return mThickness; }
    float GetProgress() const { return mProgress; }
    bool IsBarDragging() const { return mIsBarDragging; }

    void SetDuration(float duration) { mDuration = duration; }
    float GetCurrentTime() const { return mCurrentTime; }
    void SetStartTime(float startTime) { mStartTime = startTime; }

private:
    const float mThickness = 40.0f;

    float mBarWidth;
    float mBarHeight;
    float mProgress;
    bool mIsBarDragging;

    float mDuration;
    float mCurrentTime;
    float mStartTime;

    ImGuiIO mImGuiIO;
    ImVec2 mCursorPos;

    class Renderer *mRenderer;
};