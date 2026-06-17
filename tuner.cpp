#include "miniaudio.h"
#include <stdlib.h>
#include <stdio.h>
#include "audio.h"
#include <thread>
#include <mutex>
#include <condition_variable>

/*TODO:
1. Use interpolation to find exact frequecny of peak
2. Find out how ot use FFTW with floats to avoid costly cast and copy
*/

constexpr int sampleRate{1023};
constexpr int bufferFillPeriod{200};
constexpr int periodSizeInFrames{bufferFillPeriod * sampleRate / 1000}; // Number of frames in one buffer fill period
constexpr int periodFFFTSample{1};
constexpr int numberFFTSamples{sampleRate * periodFFFTSample};

bool EndThreads{false};

std::condition_variable qAudioData;
std::mutex mAudioData;

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    std::unique_lock<std::mutex> lAudioData{mAudioData};
    // Lock while updating In data of AudioObject
    AudioObject *audioPtr = static_cast<AudioObject *>(pDevice->pUserData);
    audioPtr->updateIn(static_cast<const float *>(pInput), static_cast<int>(frameCount));
    // Notify fft_thread to work on data
    qAudioData.notify_one();
}

void fft_thread(AudioObject *audioPtr)
{
    std::unique_lock<std::mutex> lAudioData{mAudioData};
    int binSize{audioPtr->m_sRate / audioPtr->m_N};
    std::cout << "bin size -> " << binSize << "\n";
    for (;;)
    {
        qAudioData.wait(lAudioData);
        // audioPtr->windowHannIn(); //cannot do this with the shift as it wil window ontop of windw etc
        audioPtr->generateOut();
        audioPtr->computeFreqMag(); // TODO need to sßmehow be able to access bin size, probelery make it part of audio obeject
        audioPtr->printFreqMag();
        audioPtr->shiftIn(audioPtr->m_fSize);

        if (EndThreads)
        {
            break;
        }
    }
}

int main(void)
{
    ma_device_config deviceConfig;
    ma_device device;
    ma_result result;
    AudioObject audio(numberFFTSamples, periodSizeInFrames, sampleRate);

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format = ma_format_f32;
    deviceConfig.capture.channels = 1;
    deviceConfig.sampleRate = audio.m_sRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.periodSizeInFrames = audio.m_fSize;
    deviceConfig.pUserData = &audio;

    std::thread tFFT(fft_thread, &audio);

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize capture device.\n");
        return -2;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS)
    {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }

    printf("Press Enter to stop recording...\n");
    getchar();

    EndThreads = true;
    qAudioData.notify_one(); // Let fft thread pass
    ma_device_uninit(&device);
    tFFT.join();

    return 0;
}