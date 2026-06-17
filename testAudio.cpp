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
    std::array<float, 10> data{0, 1, 0, -1, 0, 1, 0, -1, 0, 1};
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
    int sampleRate{4096};
    int sampleLength{200}; // ms
    int N = sampleRate * 1000 / sampleLength;
    AudioObject testAudio(N, N, sampleRate);
    AudioObject::ArrayTopFreq topFreq{0};

    std::vector<float> testData = createAddedSineWaves(1, 100, 0, 1, sampleRate, N);
    testAudio.updateIn(testData.data(), N);
    testAudio.printIn();
    testAudio.generateOut();
    testAudio.computeFreqMag();
    testAudio.sortTopFreq();
    testAudio.printTopFreq();
    topFreq = testAudio.getTopFreq();
    testAudio.quadInterpolPeak(topFreq[0].freq);
    testAudio.quadInterpolPeak(topFreq[1].freq);
}

int main()
{
    testShift();
    testFull();
}