#pragma once

#include "../external/kissfft/kiss_fft.h"

#include <cstdlib>
#include <vector>

#define FFT_SIZE 1024
class FFT
{
public:
    FFT(class Application *app);
    ~FFT();

    void Update();

private:
    int16_t *mPcm;

    kiss_fft_cfg mCfg;

    kiss_fft_cpx mInput[FFT_SIZE];
    kiss_fft_cpx mOutput[FFT_SIZE];
    std::vector<float> mSpectrum;

    class Application *mApplication;
};