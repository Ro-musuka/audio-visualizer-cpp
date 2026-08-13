#include "FFT.h"

#include <stdio.h>

#include "Application.h"
#include "AudioPlayer.h"
#include "Renderer.h"
#include "GUI.h"

FFT::FFT(Application *app)
    : mApplication(app),
      mSpectrum(FFT_SIZE / 2),
      mCfg(nullptr)
{
    mPcm = reinterpret_cast<int16_t *>(mApplication->GetAudioPlayer()->GetBuffer());

    mCfg = kiss_fft_alloc(FFT_SIZE, 0, nullptr, nullptr);

    mSpectrum.resize(FFT_SIZE / 2);
}

FFT::~FFT()
{
    free(mCfg);
}

void FFT::Update()
{
    float currentTime = mApplication->GetRenderer()->GetGUI()->GetCurrentTime();
    int freq = mApplication->GetAudioPlayer()->GetFreq();
    int channels = mApplication->GetAudioPlayer()->GetChannels();

    if (mApplication->GetAudioPlayer()->GetDuration() <= currentTime + FFT_SIZE / static_cast<float>(freq))
    {
        return;
    }

    int currentFrame = static_cast<int>(currentTime * freq);
    int currentSample = currentFrame * channels;

    // 最初の1024サンプルを解析
    for (int i = 0; i < FFT_SIZE; i++)
    {
        int index = currentSample + i * channels;
        float sample = (mPcm[index] + mPcm[index + 1]) * 0.5f / 32768.0f;

        float window = 0.5f * (1.0f - cosf(2.0f * M_PI * i / (FFT_SIZE - 1)));

        mInput[i].r = sample * window;
        mInput[i].i = 0.0f;
    }

    // FFT
    kiss_fft(mCfg, mInput, mOutput);

    for (int i = 0; i < FFT_SIZE / 2; i++)
    {
        mSpectrum[i] = sqrtf(mOutput[i].r * mOutput[i].r + mOutput[i].i * mOutput[i].i);
    }

    // 結果表示
    for (int i = 0; i < 20; i++)
    {
        printf("%d: %f\n", i, mSpectrum[i]);
    }
}