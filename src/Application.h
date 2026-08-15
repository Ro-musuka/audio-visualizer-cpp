#pragma once

class Application
{
public:
    Application();

    bool Init(const char *fileName);
    void Execution();
    void Shutdown();

    class AudioPlayer *GetAudioPlayer() const { return mAudioPlayer; }
    class Renderer *GetRenderer() const { return mRenderer; }
    class FFT *GetFFT() const { return mFFT; }

private:
    void Input();
    void Update();
    void Output();

    bool mIsRunning;
    float mTicksCount;

    // 音楽再生
    class AudioPlayer *mAudioPlayer;
    // 描画処理
    class Renderer *mRenderer;
    // フーリエ変換
    class FFT *mFFT;
};