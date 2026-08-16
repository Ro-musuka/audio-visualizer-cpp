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

    int GetChannels() const { return mSpec.channels; }
    int GetFreq() const { return mSpec.freq; }
    Uint8 *GetBuffer() const { return mBuffer; }
    Uint32 GetLength() const { return mLength; }

    float GetDuration() const { return mDuration; }
    void SetProgress(float progress);
    float GetStartTime() { return mStartTime; }

    bool GetIsPaused() const { return mIsPaused; }
    void IsPaused() { mIsPaused = true; }
    void IsPlaying() { mIsPaused = false; }

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

    bool mIsPaused;
    bool mIsPusedPrevious;

    class Application *mApplication;
};