#ifndef IO_LIBRARY_H
#define IO_LIBRARY_H

#include <portaudio.h>

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER   (512)
#define NUM_SECONDS         (5)
#define NUM_CHANNELS        (2)
#define DITHER_FLAG         (0)
#define WRITE_TO_FILE       (1)

#if 1
#define PA_SAMPLE_TYPE      paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE      (0.0f)
#define PRINTF_S_FORMAT     "%.8f"
#elif 1
#define PPA_SAMPLE_TYPE     paInt16
typedef short               SAMPLE;
#define SAMPLE_SILENCE      (0)
#define PPRINTF_S_FORMAT    "%d"
#elif 0
#define PPA_SAMPLE_TYPE     paInt8
typedef char                SAMPLE;
#define SAMPLE_SILENCE      (0)
#define PPRINTF_S_FORMAT    "%d"
#else
#define PPA_SAMPLE_TYPE     paUInt8
typedef unsigned char       SAMPLE;
#define SSAMPLE_SILENCE     (128)
#define PPRINTF_S_FORMAT    "%d"
#endif

typedef struct
{
    unsigned int    frameIndex;
    unsigned int    maxFrameIndex;
    SAMPLE          *audioBuffer;
} audioData;

class IO {

private:
    static int recordCallback (const void                      *inputBuffer,
                               void                            *outputBuffer,
                               unsigned long                   framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags           statusFlags,
                               void                            *userData);

    static int playCallback(const void *inputBuffer,
                            void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

    static PaError done(void *userData,
                        PaError err);

public:
    static int record(PaStream* stream,
                      audioData* data,
                      int numSeconds,
                      int sampleRate);

    static int play(PaStream* stream,
                    audioData* data,
                    int sampleRate);
};

#endif //IO_LIBRARY_H