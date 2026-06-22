#include "audio.h"
#include "linearSystem.h"

void AudioObject::updateIn(const float *in, int sizeIn)
{

    // difference between sizeIn and N, offest = m_N - sizeIn -> m_in[i+offset] -> put newest data at m_in[m_N]
    int offset{m_N - sizeIn};
    for (int i{0}; i < sizeIn; i++)
    {
        m_in[i + offset] = static_cast<double>(in[i]);
    }
}

void AudioObject::shiftIn(int sizeShift)
{
    // want m_in.begin to be updated to [m_N - sizeShift]
    std::rotate(m_in.begin(), m_in.begin() + sizeShift, m_in.end());
}

void AudioObject::windowHannIn(void)
{
    for (int i = 0; i < m_N; i++)
    {
        m_in[i] = m_in[i] * m_hann[i];
    }
}

void AudioObject::removeWindowHannIn(void)
{
    for (int i = 0; i < m_N; i++)
    {
        m_in[i] = m_in[i] * m_hannInv[i];
    }
}

void AudioObject::generateOut(void)
{
    fftw_execute(m_p);
}

void AudioObject::computeFreqMag(void)
{
    // No useful information in bin N/2 and higher as is nyquit frequncy
    for (int i = 0; i <= m_N / 2; i++)
    {
        m_freq[i].mag = std::abs(m_out[i]);
        m_freq[i].freq = i * m_binSize;
    }
}

double AudioObject::findFreqMag(int f_freq)
{
    for (const auto &freq : m_freq)
    {
        if (f_freq == freq.freq)
        {
            return freq.mag;
        }
    }
    return 0;
}

void AudioObject::quadInterpolPeak(double freq)
{
    linearSystem3X3<double>::Array2D<double, 3, 1> x;
    linearSystem3X3<double> sys;
    const double a1{freq};
    const double a2{freq - m_binSize};
    const double a3{freq + m_binSize};
    const double b1 = findFreqMag(a1);
    const double b2 = findFreqMag(a2);
    const double b3 = findFreqMag(a3);

    // quadratic equation ax^2 + bx + c, solving for a, b, and c
    sys.addEquation(a1 * a1, a1, 1, b1, 0);
    sys.addEquation(a2 * a2, a2, 1, b2, 1);
    sys.addEquation(a3 * a3, a3, 1, b3, 2);
    sys.solveCramers();
    x = sys.getx();
    // local maxima is at -b/2a
    std::cout << (-1 * x[1][0] / (2 * x[0][0])) << "\n";
}

void AudioObject::sortTopFreq(void)
{
    std::sort(m_freq.begin(), m_freq.end());
    for (int i{0}; i < m_tFreq.size(); i++)
    {
        m_tFreq[i] = m_freq[i];
    }
}

const AudioObject::ArrayTopFreq &AudioObject::getTopFreq(void)
{
    return m_tFreq;
}

void AudioObject::printTopFreq(void)
{
    for (const auto &freq_d : m_tFreq)
    {
        std::cout << freq_d.freq << " ";
    }

    std::cout << "\n";
}

void AudioObject::printIn(void)
{
    for (const double &i : m_in)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

void AudioObject::printOut(void)
{
    for (const auto &i : m_out)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}