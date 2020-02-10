#include <cstdio>
#include <cmath>
#include "lib/IO/library.h"
#include <essentia/algorithmfactory.h>
#include <essentia/streaming/algorithms/poolstorage.h>
#include <essentia/scheduler/network.h>

#define CONV 0.01
#define ORDER 21
#define NUM_SAMP 512
#define SAMP_FREQ 44100
#define PI 3.1315926
#define DESIRED 0
#define NOISE sin(2*PI*T*1000/SAMP_FREQ)

using namespace IO;
using namespace std;
using namespace essentia;
using namespace essentia::streaming;
using namespace essentia::scheduler;

int main() {
    long I, T;
    double D, Y, E;
    double W[ORDER+1] = {0.0};
    double X[ORDER+1] = {0.0};

    int hopSize = 1024;
    int frameSize = 2048;
    int sampleRate = 44100;

    FILE *desired, *Y_out, *error;
    audioData audioSample;
    PaStream* stream;

    essentia::init();

    IO::record(stream, &audioSample, 5, sampleRate);

    // IO:play(stream, &audioSample, sampleRate);

    std::vector<SAMPLE> audioBuffer(audioSample.audioBuffer, audioSample.audioBuffer + audioSample.maxFrameIndex);

    Pool pool;

    AlgorithmFactory& factory = streaming::AlgorithmFactory::instance();

    Algorithm* frame = factory.create("FrameCreator",
                                      "frameSize", frameSize,
                                      "hopSize", hopSize);

    Algorithm* window = factory.create("Windowing",
                                       "normalized", true,
                                       "size", frameSize,
                                       "type", "hann",
                                       "zeroPadding", 0,
                                       "zeroPhase", true);
    
    Algorithm* fft = factory.create("FFT",
                                    "size", frameSize);

    cout << "---------- Connecting the Essentia algorithms ----------" << endl;

    desired = fopen("DESIRED", "w++");
    Y_out = fopen("Y_OUT", "w++");
    error = fopen("ERROR", "w++");

    for(T = 0; T < NUM_SAMP; T++) {
        X[0] = NOISE;
        D = DESIRED;
        Y = 0;

        for (I = 0; I <= ORDER; I++) {
            Y += (W[I] * X[I]);
        }

        E = D - Y;

        for (I = ORDER; I >= 0; I--) {
            W[I] = W[I] + (CONV * E * X[I]);
            if (I != 0) {
                X[I] = X[I - 1];
            }
        }

        fprintf(desired, "\n%10g    %10f", (float)T/SAMP_FREQ, D);
        fprintf(Y_out, "\n%10g    %10f", (float)T/SAMP_FREQ, Y);
        fprintf(error, "\n%10g    %10f", (float)T/SAMP_FREQ, E);
    }

    fclose(desired);
    fclose(Y_out);
    fclose(error);
}
