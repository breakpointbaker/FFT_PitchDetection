# FFT_PitchDetection
Attempt to use FFT to identify the most prominent frequencies in an audio signal, for the purpose of tuning musical instruments. 
# Note: 
Underdevelopment
# Build:
g++ tuner.cpp miniaudio.c audio.cpp -pthread -lfftw3 -o out
