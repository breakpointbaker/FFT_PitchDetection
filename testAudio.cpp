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
    float mag1 = 0.9;
    float freq1 = 440.6;
    float mag2 = 0.8;
    float freq2 = 82.4;

    int sampleRate{1024};
    int sampleLength{1000}; // ms
    int N = sampleRate * sampleLength / 1000;

    // Test 1 - with shift and windowing and removing wnindow
    int framesize = N / 4;
    AudioObject testAudio(N, framesize, sampleRate);
    AudioObject::ArrayTopFreq topFreq{0};

    std::vector<float> testData = createAddedSineWaves(mag1, freq1, mag2, freq2, sampleRate, N);

    for (int i{0}; i < (N / framesize); i++)
    {
        testAudio.updateIn(&testData[i * framesize], framesize);
        testAudio.windowHannIn();
        testAudio.generateOut();
        testAudio.computeFreqMag();
        testAudio.removeWindowHannIn();
        testAudio.shiftIn(framesize);
    }
    testAudio.sortTopFreq();
    topFreq = testAudio.getTopFreq();
    std::cout << "With shift and winowing and removing windowing" << "\n";
    testAudio.quadInterpolPeak(topFreq[0].freq);
    testAudio.quadInterpolPeak(topFreq[1].freq);
    testAudio.quadInterpolPeak(topFreq[2].freq);

    // Test 2 - no shifting or removing window, should be the same as before
    AudioObject testAudio2(N, N, sampleRate);
    testAudio2.updateIn(testData.data(), N);
    testAudio2.windowHannIn();
    testAudio2.generateOut();
    testAudio2.computeFreqMag();
    testAudio2.sortTopFreq();
    topFreq = testAudio2.getTopFreq();
    std::cout << "Without shift or removing windowing - should be the same as above" << "\n";
    testAudio2.quadInterpolPeak(topFreq[0].freq);
    testAudio2.quadInterpolPeak(topFreq[1].freq);
    testAudio2.quadInterpolPeak(topFreq[2].freq);
}

int main()
{
    testShift();
    testFull();
}