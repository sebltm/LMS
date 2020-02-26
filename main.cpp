#include <cstdio>
#include <cmath>
#include <IO.h>
#include "FeatureExtraction.h"

#define CONV 0.01
#define ORDER 21
#define NUM_SAMP 512
#define SAMP_FREQ 44100
#define DESIRED 0
#define NOISE sin(2*PI*T*1000/SAMP_FREQ)

using namespace std;

int main() {
    long i, T;
    double desiredSignal, outputSignal, errorSignal;
    double weights[ORDER + 1] = {0.0};
    double noiseSignal[ORDER+1] = {0.0};

    int hopSize = 1024;
    int frameSize = 2048;
    int sampleRate = 44100;

    FILE *desired, *Y_out, *error;
//    audioData audioSample;
//    PaStream* stream;
//
//    // Record a 5s sample
//    IO::record(stream, &audioSample, 5, sampleRate);
//
//    // IO:play(stream, &audioSample, sampleRate);
//
//    std::vector<SAMPLE> audioBuffer(audioSample.audioBuffer, audioSample.audioBuffer + audioSample.maxFrameIndex);
//
//    int windowSize = 2048;
//
//    auto fft_array = FeatureExtraction::STFT(&audioBuffer, audioSample.maxFrameIndex, windowSize, 50);
//
//    FILE *fft_output = fopen("FFT_OUT", "w++");
//
//    for(int i = 0; i < windowSize / 2 + 1; i++) {
//        fprintf(fft_output, "\n%f, %f", fft_array[i][0], fft_array[i][1]);
//        free(fft_array[i]);
//    }
//
//    free(fft_array);
//    fclose(fft_output);

    desired = fopen("DESIRED", "w++");

    if(desired == nullptr) {
        fprintf(stdout, "Error while open file DESIRED: %d", errno);
        return errno;
    }

    Y_out = fopen("Y_OUT", "w++");

    if(Y_out == nullptr) {
        fprintf(stdout, "Error while open file Y_OUT: %d", errno);
        fclose(desired);
        return errno;
    }

    error = fopen("ERROR", "w++");

    if(error == nullptr) {
        fprintf(stdout, "Error while open file ERROR: %d", errno);
        fclose(desired);
        fclose(Y_out);
        return errno;
    }

    for(T = 0; T < NUM_SAMP; T++) {
        noiseSignal[0] = NOISE;
        desiredSignal = DESIRED;
        outputSignal = 0;

        for (i = 0; i <= ORDER; i++) {
            outputSignal += (weights[i] * noiseSignal[i]); // Output filter
        }

        errorSignal = desiredSignal - outputSignal;

        for (i = ORDER; i >= 0; i--) {
            weights[i] = weights[i] + (CONV * errorSignal * noiseSignal[i]); // Update weights
            if (i != 0) {
                noiseSignal[i] = noiseSignal[i - 1];
            }
        }

        fprintf(desired, "\n%10g    %10f", (float)T/SAMP_FREQ, desiredSignal);
        fprintf(Y_out, "\n%10g    %10f", (float)T/SAMP_FREQ, outputSignal);
        fprintf(error, "\n%10g    %10f", (float)T/SAMP_FREQ, errorSignal);
    }

    fclose(desired);
    fclose(Y_out);
    fclose(error);
}
