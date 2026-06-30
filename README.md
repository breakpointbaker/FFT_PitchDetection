# FFT_PitchDetection
Attempt to use FFT to identify the most prominent frequencies in an audio signal, for the purpose of tuning musical instruments. 
# Note: 
- Underdevelopment
- Requires miniaudio.c and miniaudio.h
# TODO:
- Find ideal data size for FFTW
- Try different windowing functions
- Add filtering to boost frequencies in desired range, 80Hz - 400Hz
# Build:
g++ tuner.cpp miniaudio.c audio.cpp linearSystem.cpp -pthread -lfftw3 -o out \
g++ testAudio.cpp audio.cpp linearSystem.cpp -lfftw3 -o out
