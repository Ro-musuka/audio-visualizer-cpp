#include "GUI.h"

#include <SDL3/SDL.h>
#include <GL/glew.h>

#include <algorithm>

#include "Renderer.h"
#include "Application.h"
#include "AudioPlayer.h"

GUI::GUI(Renderer *renderer)
    : mRenderer(renderer),
      mProgress(0.0f),
      mIsBarDragging(false)
{
    mScreenWidth = mRenderer->GetScreenWidth();
    mScreenHeight = mRenderer->GetScreenHeight();
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();
}

bool GUI::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    mImGuiIO = ImGui::GetIO();
    mImGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL(mRenderer->GetWindow(), mRenderer->GetContext());
    ImGui_ImplOpenGL3_Init("#version 410");

    mBarWidth = mRenderer->GetScreenWidth() / 16 * 9;
    mBarHeight = mThickness / 2;

    return true;
}

void GUI::ProcessEvent(SDL_Event event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);

    if (event.type == SDL_EVENT_WINDOW_RESIZED)
    {
        mScreenWidth = event.window.data1;
        mScreenHeight = event.window.data2;
    }
}

void GUI::Update(float deltaTime)
{
    if (!mRenderer->GetApplication()->GetAudioPlayer()->IsPused())
    {
        mCurrentTime += deltaTime;
    }

    if (!mIsBarDragging)
    {
        mProgress = std::clamp(mCurrentTime / mDuration, 0.0f, 1.0f);
        if (mDuration == 0)
        {
            SDL_Log("総再生時間が0秒です。 mDuration: %f\nmProgressを0.0fに設定します。", mDuration);
            mProgress = 0.0f;
        }
    }
}

void GUI::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();

    // 位置
    ImGui::SetNextWindowPos(ImVec2(0, mScreenHeight - mThickness), ImGuiCond_Always);
    // サイズ
    ImGui::SetNextWindowSize(ImVec2(mScreenWidth, mThickness), ImGuiCond_Always);

    // ウィンドウ作成
    // サイズ・位置変更不可、titleBarなし
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    mCursorPos = ImGui::GetCursorScreenPos();

    // ボタン判定
    ImGui::InvisibleButton(" ", {mBarWidth, mBarHeight});

    // 再生バーのI/O機能
    if (ImGui::IsItemActive())
    {
        float mouseX = ImGui::GetIO().MousePos.x;
        mProgress = (mouseX - mCursorPos.x) / mBarWidth;
        mProgress = std::clamp(mProgress, 0.0f, 1.0f);

        mIsBarDragging = true;
    }
    // バー操作が終了した場合
    else if (mIsBarDragging)
    {
        mRenderer->GetApplication()->GetAudioPlayer()->SetProgress(mProgress);
        mRenderer->GetApplication()->GetAudioPlayer()->GetStartTime();
        mCurrentTime = mProgress * mDuration;

        mIsBarDragging = false;
    }

    // 再生バーの作成
    ImDrawList *draw = ImGui::GetWindowDrawList();

    float currentPoint = mCursorPos.x + mProgress * mBarWidth;
    // 灰色バー
    // 位置、サイズ、カラー
    draw->AddRectFilled({mCursorPos.x, mCursorPos.y + mThickness / 4},
                        {mCursorPos.x + mBarWidth, mCursorPos.y + 4 + mThickness / 4},
                        IM_COL32(80, 80, 80, 255));
    // 再生済みバー
    draw->AddRectFilled({mCursorPos.x, mCursorPos.y + mThickness / 4},
                        {currentPoint, mCursorPos.y + 4 + mThickness / 4},
                        IM_COL32(255, 80, 60, 255));
    // 現在再生位置の点
    draw->AddCircleFilled({currentPoint, mCursorPos.y + 2 + mThickness / 4},
                          6.0f,
                          IM_COL32(255, 255, 255, 255));

    // 同じ行に描画
    ImGui::SameLine();
    // 現在の再生時刻
    int duration = static_cast<int>(mDuration);
    int currentTime = static_cast<int>(mCurrentTime);

    // min:sec を出力
    ImGui::Text(" %d:%02d / %d:%02d", currentTime / 60, currentTime % 60, duration / 60, duration % 60);

    ImGui::End();

    // ImGui描画
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}