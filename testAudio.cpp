#include "audio.h"

std::vector<float> createAddedSineWaves(float mag1, float freq1, float mag2, float freq2,
                                        int sampleRate, int sampleLength)
{
    const double PI{3.1416};
    std::vector<float> output(sampleLength);

    for (int n = 0; n < sampleLength; n++)
    {
        double time = static_cast<double>(n) / sampleRate;
        float sample = mag1 * std::sin(2 * PI * freq1 * time) +
                       mag2 * std::sin(2 * PI * freq2 * time);
        output[n] = sample;
    }

    return output;
}

void testShift(void)
{
    int N{10};
    int fSize{2};
    int sampleRate{1024};
    AudioObject testAudio(N, fSize, sampleRate);
    std::array<float, 10> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    testAudio.updateIn(data.data(), fSize);
    testAudio.printIn();
    for (int i{1}; i < 5; i++)
    {
        testAudio.shiftIn(fSize);
        testAudio.printIn();
        testAudio.updateIn(&data[i * 2], fSize);
        testAudio.printIn();
    }
    testAudio.shiftIn(fSize);
    testAudio.printIn();
    testAudio.updateIn(&data[2], fSize);
    testAudio.printIn();
}

void testFull(void)
{
    float mag1 = 1;
    float freq1 = 1001.6;
    float mag2 = 0.8;
    float freq2 = 82.4;

    int sampleRate{4096};
    int sampleLength{200}; // ms
    int N = sampleRate * sampleLength / 1000;
    AudioObject testAudio(N, N, sampleRate);
    AudioObject::ArrayTopFreq topFreq{0};

    std::vector<float> testData = createAddedSineWaves(mag1, freq1, mag2, freq2, sampleRate, N);
    testAudio.updateIn(testData.data(), N);
    testAudio.windowHannIn();
    testAudio.generateOut();
    testAudio.computeFreqMag();
    testAudio.sortTopFreq();
    testAudio.printTopFreq();
    topFreq = testAudio.getTopFreq();
    testAudio.quadInterpolPeak(topFreq[0].freq);
    testAudio.quadInterpolPeak(topFreq[1].freq);
    testAudio.quadInterpolPeak(topFreq[2].freq);
}

int main()
{
    testShift();
    testFull();
}