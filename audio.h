#ifndef AUDIO_H_
#define AUDIO_H_

#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <complex>
#include <algorithm>
#include "miniaudio.h"

class AudioObject
{
public:
    struct freq_data
    {
        double mag;
        double freq;

        bool operator<(const freq_data &a) const
        {
            return mag > a.mag;
        }
    };

    using ArrayTopFreq = std::array<freq_data, 3>;

    const double PI{3.1416};
    const int m_N;
    const int m_fSize;
    const int m_sRate;
    const double m_binSize;

private:
    std::vector<double> m_in;
    std::vector<std::complex<double>> m_out;
    std::vector<freq_data> m_freq;
    ArrayTopFreq m_tFreq{0};
    std::vector<double> m_hann;
    fftw_plan m_p;

public:
    AudioObject(int N, int fSize, int sRate)
        : m_in(N), m_out(N / 2 + 1), m_freq(N / 2 + 1), m_hann(N), m_N{N}, m_fSize{fSize}, m_sRate{sRate}, m_binSize{static_cast<double>(sRate) / N}
    {
        for (int i = 0; i < m_N; i++)
        {
            m_hann[i] = 0.5 * (1 - std::cos(2 * PI * i / (m_N - 1)));
        }
        m_p = fftw_plan_dft_r2c_1d(m_N, m_in.data(), reinterpret_cast<fftw_complex *>(m_out.data()), FFTW_ESTIMATE);
    }
    ~AudioObject()
    {
        fftw_destroy_plan(m_p);
    }

    void updateIn(const float *in, int sizeIn);
    void shiftIn(int sizeShift);
    void windowHannIn(void);
    void generateOut(void);
    void computeFreqMag(void);
    double findFreqMag(int f_freq);
    void quadInterpolPeak(double freq);
    void sortTopFreq(void);
    const ArrayTopFreq &getTopFreq(void);
    void printTopFreq(void);
    void printIn(void);
    void printOut(void);
};

#endif