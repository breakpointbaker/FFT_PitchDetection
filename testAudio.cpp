#include "audio.h"

int main()
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