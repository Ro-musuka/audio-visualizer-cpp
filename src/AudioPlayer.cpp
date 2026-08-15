#include "AudioPlayer.h"

#include <algorithm>

#include "Application.h"

AudioPlayer::AudioPlayer(Application *app)
    : mDuration(0.0f),
      mStartTime(0.0f),
      mIsPaused(true),
      mIsPusedPrevious(true),
      mApplication(app)
{
}

AudioPlayer::~AudioPlayer()
{
    SDL_CloseAudioDevice(mDeviceID);
    SDL_DestroyAudioStream(mAudioStream);
    SDL_free(mBuffer);
}

bool AudioPlayer::Init(const char *musicFile)
{
    if (!SDL_LoadWAV(musicFile, &mSpec, &mBuffer, &mLength))
    {
        SDL_Log("%sの読み込みに失敗しました。: %s", musicFile, SDL_GetError());
        return false;
    }

    SDL_AudioSpec dst = mSpec;
    dst.format = SDL_AUDIO_F32;

    mAudioStream = SDL_CreateAudioStream(&mSpec, &dst);

    mDeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &dst);
    SDL_BindAudioStream(mDeviceID, mAudioStream);

    int bytesPerSample = SDL_AUDIO_BITSIZE(mSpec.format) / 8;
    mBytesPerFrame = mSpec.channels * bytesPerSample;
    int totalFrames = mLength / mBytesPerFrame;

    //  総フレーム数 / Hz で再生時間を求める
    mDuration = static_cast<float>(totalFrames) / mSpec.freq;

    SDL_PutAudioStreamData(mAudioStream, mBuffer, mLength);
    SDL_PauseAudioDevice(mDeviceID);

    return true;
}

void AudioPlayer::Play()
{
    if (mIsPusedPrevious == mIsPaused)
        return;

    if (mIsPaused)
    {
        Pause();
    }
    else
    {
        Resume();
    }

    mIsPusedPrevious = mIsPaused;
}

void AudioPlayer::SetProgress(float progress)
{
    Uint32 offset = static_cast<Uint32>(mLength * progress);
    offset -= offset % mBytesPerFrame;

    SDL_ClearAudioStream(mAudioStream);

    if (offset < mLength)
    {
        SDL_PutAudioStreamData(mAudioStream, mBuffer + offset, mLength - offset);

        int frames = offset / mBytesPerFrame;
        mStartTime = static_cast<float>(frames) / mSpec.freq;
    }

    mIsPaused = false;
}

void AudioPlayer::Resume()
{
    SDL_ResumeAudioDevice(mDeviceID);
}

void AudioPlayer::Pause()
{
    SDL_PauseAudioDevice(mDeviceID);
}