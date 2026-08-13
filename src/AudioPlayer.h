#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

class AudioPlayer
{
public:
    AudioPlayer(class Application *app);
    ~AudioPlayer();

    bool Init(const char *musicFile);

    void Play();

    // ゲッター/セッター
    const SDL_AudioStream *GetAudioStream() const { return mAudioStream; }

    float GetDuration() const { return mDuration; }
    void SetProgress(float progress);
    float GetStartTime() { return mStartTime; }

    bool IsPused() const { return mIsPused; }
    void SetIsPused(bool state) { mIsPused = state; }

private:
    void Resume();
    void Pause();

    SDL_AudioStream *mAudioStream;
    SDL_AudioDeviceID mDeviceID;

    SDL_AudioSpec mSpec;
    Uint8 *mBuffer;
    Uint32 mLength;

    int mBytesPerFrame;
    float mDuration;
    float mStartTime;

    bool mIsPused;
    bool mIsPusedPrevious;

    class Application *mApplication;
};