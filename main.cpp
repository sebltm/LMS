#include <cstdio>
#include <cmath>
#include <IO.h>
#include "FeatureExtraction.h"

#define CONV 0.01f
#define ORDER 500
#define NUM_SAMP 512
#define NUM_SECONDS 5
#define SAMP_FREQ 44100
#define DESIRED cos(2*PI*T*1000/SAMP_FREQ)/2
#define NOISE sin(2*PI*T*1000/SAMP_FREQ)

using namespace std;

int main() {
    long i, T;
    double desiredSignal, filterOutput, errorSignal;
    double weights[ORDER + 1] = {0.0};
    double noiseSignal[ORDER + 1] = {0.0};

    int hopSize = 1024;
    int frameSize = 2048;
    int sampleRate = 44100;
    int numSeconds = 5;

    FILE *desired, *Y_out, *error, *noise;
    audioData audioSample;
    audioData noiseData, musicData, filterData;

    string filename = "try.RAW";

    // Record a 5s sample
    IO::record(&audioSample, numSeconds, sampleRate, filename.c_str());

    IO::play(&audioSample, sampleRate);

    std::vector<SAMPLE> audioBuffer(audioSample.audioBuffer, audioSample.audioBuffer + audioSample.maxFrameIndex);

    int windowSize = 2048;

    auto fft_array = FeatureExtraction::STFT(&audioBuffer, audioSample.maxFrameIndex, windowSize, 50);

    FILE *fft_output = fopen("FFT_OUT", "w++");

    for(i = 0; i < windowSize / 2 + 1; i++) {
        // fprintf(fft_output, "\n%f, %f", fft_array[i][0], fft_array[i][1]);
        free(fft_array[i]);
    }

    free(fft_array);
    fclose(fft_output);

    desired = fopen("music.RAW", "r");

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

    noise = fopen("noise.RAW", "r");

    if(noise == nullptr) {
        fprintf(stdout, "Error while open file NOISE: %d", errno);
        fclose(desired);
        fclose(Y_out);
        fclose(error);
        return errno;
    }

    noiseData.maxFrameIndex = numSeconds * sampleRate;
    noiseData.audioBuffer = (SAMPLE*)malloc(noiseData.maxFrameIndex * NUM_CHANNELS * sizeof(SAMPLE));
    if(noiseData.audioBuffer == nullptr) {
        fprintf(stdout, "Error while allocating memory for noise buffer");
        fclose(desired);
        fclose(Y_out);
        fclose(error);
        fclose(noise);

        return -2;
    }
    fread(noiseData.audioBuffer, NUM_CHANNELS * sizeof(SAMPLE), noiseData.maxFrameIndex, noise);

    musicData.maxFrameIndex = numSeconds * sampleRate;
    musicData.audioBuffer = (SAMPLE*)malloc(musicData.maxFrameIndex * NUM_CHANNELS * sizeof(SAMPLE));
    if(musicData.audioBuffer == nullptr) {
        fprintf(stdout, "Error while allocating memory for music buffer");
        fclose(desired);
        fclose(Y_out);
        fclose(error);
        fclose(noise);
        free(noiseData.audioBuffer);

        return -2;
    }
    fread(musicData.audioBuffer, NUM_CHANNELS * sizeof(SAMPLE), musicData.maxFrameIndex, desired);


    filterData.maxFrameIndex = numSeconds * sampleRate;
    filterData.audioBuffer = (SAMPLE*)malloc(filterData.maxFrameIndex * NUM_CHANNELS * sizeof(SAMPLE));
    if(filterData.audioBuffer == nullptr) {
        fprintf(stdout, "Error while allocating memory for music buffer");
        fclose(desired);
        fclose(Y_out);
        fclose(error);
        fclose(noise);
        free(noiseData.audioBuffer);
        free(musicData.audioBuffer);

        return -2;
    }

    for(T = 0; T < numSeconds * sampleRate; T++) {

        noiseSignal[0] = noiseData.audioBuffer[T];
        desiredSignal = -noiseData.audioBuffer[T];
        filterOutput = 0;

        for (i = 0; i <= ORDER; i++) {
            filterOutput += (weights[i] * noiseSignal[i]); // Output filter
        }

        errorSignal = desiredSignal - filterOutput;

        for (i = ORDER; i >= 0; i--) {
            weights[i] = weights[i] + (CONV * errorSignal * noiseSignal[i]); // Update weights
            if (i != 0) {
                noiseSignal[i] = noiseSignal[i - 1];
            }
        }

        filterData.audioBuffer[T] = filterOutput;

        fprintf(desired, "\n%10g    %10f", (float)T/SAMP_FREQ, desiredSignal);
        fprintf(Y_out, "\n%10g    %10f", (float)T/SAMP_FREQ, filterOutput);
        fprintf(error, "\n%10g    %10f", (float)T/SAMP_FREQ, errorSignal);
    }

    FILE *fid;
    fid = fopen("filterOutput.RAW", "wb");
    if(fid == nullptr) {
        fprintf(stdout, "Could not open file");
    } else {
        fwrite(filterData.audioBuffer, NUM_CHANNELS * sizeof( SAMPLE ), NUM_SECONDS * sampleRate, fid);
        fclose(fid);
    }

    free(noiseData.audioBuffer);
    free(musicData.audioBuffer);
    free(filterData.audioBuffer);
    fclose(desired);
    fclose(Y_out);
    fclose(error);
    fclose(noise);
}
