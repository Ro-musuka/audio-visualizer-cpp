#include "FFT.h"

#include <SDL3/SDL.h>
#include "../external/kissfft/kiss_fft.h"

#include <cstdlib>
#include <vector>

FFT::FFT()
{
}

void FFT::Update(float deltaTime)
{
    // 16bit PCMとして扱う
    int16_t *pcm = reinterpret_cast<int16_t *>(buffer);
    int sampleCount = length / sizeof(int16_t);

    // floatへ変換
    std::vector<float> samples(sampleCount);

    for (int i = 0; i < sampleCount; i++)
    {
        samples[i] = pcm[i] / 32768.0f;
    }

    // kissFFT初期化
    kiss_fft_cfg cfg = kiss_fft_alloc(FFT_SIZE, 0, nullptr, nullptr);

    kiss_fft_cpx in[FFT_SIZE];
    kiss_fft_cpx out[FFT_SIZE];

    // 最初の1024サンプルを解析
    for (int i = 0; i < FFT_SIZE; i++)
    {
        float window = 0.5f * (1.0f - cosf(2.0f * M_PI * i / (FFT_SIZE - 1)));

        in[i].r = samples[i] * window;
        in[i].i = 0.0f;
    }

    // FFT
    kiss_fft(cfg, in, out);

    // 振幅計算
    std::vector<float> spectrum(FFT_SIZE / 2);

    for (int i = 0; i < FFT_SIZE / 2; i++)
    {
        spectrum[i] = sqrtf(
            out[i].r * out[i].r +
            out[i].i * out[i].i);
    }

    // 結果表示
    for (int i = 0; i < 20; i++)
    {
        SDL_Log("%d: %f\n", i, spectrum[i]);
    }
}