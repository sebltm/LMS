#include "library.h"
#include <pa_linux_alsa.h>
#include <cstdio>
#include <cstdlib>


namespace IO {

    static int recordCallback (const void                      *inputBuffer,
                               void                            *outputBuffer,
                               unsigned long                   framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags           statusFlags,
                               void                            *userData) {

        auto *data          = (audioData*)userData;
        const auto *rptr    = (const SAMPLE*)inputBuffer;
        SAMPLE *wptr        = &data->audioBuffer[data->frameIndex * NUM_CHANNELS];
        unsigned long framesToCalc;
        long i;
        int finished;
        unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

        (void) outputBuffer;
        (void) timeInfo;
        (void) statusFlags;
        (void) userData;

        if(framesLeft < framesPerBuffer) {
            framesToCalc = framesLeft;
            finished = paComplete;
        } else {
            framesToCalc = framesPerBuffer;
            finished = paContinue;
        }

        if(inputBuffer == nullptr) {
            for (i=0; i<framesToCalc; i++)
            {
                *wptr++ = SAMPLE_SILENCE;
                if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;
            }
        } else {
            for (i=0; i<framesToCalc; i++)
            {
                *wptr++ = *rptr++;
                if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;
            }
        }
        data->frameIndex += framesToCalc;
        return finished;
    }

    static int playCallback(const void *inputBuffer,
                            void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
    {
        auto *data = (audioData*)userData;
        SAMPLE *rptr = &data->audioBuffer[data->frameIndex * NUM_CHANNELS];
        auto *wptr = (SAMPLE*)outputBuffer;
        int i;
        int finished;
        unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

        (void) inputBuffer;
        (void) timeInfo;
        (void) statusFlags;
        (void) userData;

        if(framesLeft < framesPerBuffer) {

            for(i=0; i<framesLeft; i++) {
                *wptr++ = *rptr++;
                if(NUM_CHANNELS == 2) *wptr++ = *rptr++;
            }

            for ( ; i<framesPerBuffer; i++) {
                *wptr++ = 0;
                if(NUM_CHANNELS == 2) *wptr++ = 0;
            }

            data->frameIndex += framesLeft;
            finished = paComplete;
        } else {

            for(i=0; i<framesPerBuffer; i++) {
                *wptr++= *rptr++;
                if(NUM_CHANNELS == 2) *wptr++ = *rptr++;
            }

            data->frameIndex += framesPerBuffer;
            finished = paContinue;
        }
        return finished;
    }

    PaError done(void *userData,
                  PaError err)
    {
        Pa_Terminate();

        auto *data = (audioData*)userData;
        if(data->audioBuffer) {
            free(data->audioBuffer);
        }

        if( err != paNoError ) {
            fprintf( stderr, "An error occured while using the portaudio stream\n");
            fprintf( stderr, "Error number: %d\n", err);
            fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ));
        }
        return err;
    }

    int record(PaStream* stream,
               audioData* data,
               int numSeconds,
               int sampleRate) {

        int i = 0;
        unsigned int totalFrames;
        data->maxFrameIndex = totalFrames = numSeconds * sampleRate;
        data->frameIndex = 0;
        unsigned int numSamples = totalFrames * NUM_CHANNELS;
        unsigned int numBytes = numSamples * sizeof(SAMPLE);
        PaStreamParameters inputParameters;
        PaError err = paNoError;

        data->audioBuffer = (SAMPLE*) malloc(numBytes);
        if(data->audioBuffer == nullptr) {
            fprintf(stdout, "Could not allocate record array.\n");
            return done(data, err);
        }

        for(i = 0; i<numSamples; i++) {
            data->audioBuffer[i] = 0;
        }

        err = Pa_Initialize();
        if(err != paNoError) {
            return done(data, err);
        }

        inputParameters.device = Pa_GetDefaultInputDevice();
        if(inputParameters.device == paNoDevice) {
            fprintf(stderr, "Error: No default input device.\n");
            fprintf(stderr, "Found %d devices.", Pa_GetDeviceCount());
            return done(&data, err);
        }

        inputParameters.channelCount = 2;
        inputParameters.sampleFormat = PA_SAMPLE_TYPE;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
                &stream,
                &inputParameters,
                nullptr,
                sampleRate,
                FRAMES_PER_BUFFER,
                paClipOff,
                recordCallback,
                data);

        if(err != paNoError) {
            return done(data, err);
        }

        err = Pa_StartStream(stream);
        if(err != paNoError) {
            return done(&data, err);
        }
        fprintf(stdout, "\n=== Now recording ! Please speak into the microphone. ===\n");
        fflush(stdout);

        while((err = Pa_IsStreamActive(stream)) == 1) {
            Pa_Sleep(1000);
            fprintf(stdout, "Index %d.\n", data->frameIndex);
            fflush(stdout);
        }

        if(err < 0) {
            return done(data, err);
        }

    #if WRITE_TO_FILE
        {
            FILE *fid;
            fid = fopen("recorded.raw", "wb");
            if(fid == nullptr) {
                fprintf(stdout, "Could not open file");
            } else {
                fwrite( data->audioBuffer, NUM_CHANNELS * sizeof( SAMPLE ), totalFrames, fid );
                fclose(fid);
            }
        }
    #endif

        return 0;
    }

    int play(PaStream* stream,
                 audioData* data,
                 int sampleRate) {

        PaError err = paNoError;

        data->frameIndex = 0;

        //PaAlsa_EnableRealtimeScheduling(&stream, true);

        PaStreamParameters outputParameters;
        outputParameters.device = Pa_GetDefaultOutputDevice();
        if(outputParameters.device == paNoDevice) {
            fprintf(stdout, "Error: No default output device.\n");
            return done(data, err);
        }
        outputParameters.channelCount = 2;
        outputParameters.sampleFormat = PA_SAMPLE_TYPE;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        fprintf(stdout, "\n=== Now playing back ===\n");
        fflush(stdout);

        err = Pa_OpenStream(
                &stream,
                nullptr,
                &outputParameters,
                sampleRate,
                FRAMES_PER_BUFFER,
                paClipOff,
                playCallback,
                data);

        if(err != paNoError) {
            return done(data, err);
        }

        if(stream) {
            err = Pa_StartStream(stream);
            if(err != paNoError) {
                return done(data, err);
            }

            fprintf(stdout, "Waiting for playback to finish.\n");
            fflush(stdout);

            while((err = Pa_IsStreamActive(stream)) == 1) {
                Pa_Sleep(100);
            }

            if(err < 0) {
                return done(data, err);
            }

            err = Pa_CloseStream(stream);
            if(err != paNoError) {
                return done(data, err);
            }
        }
        return 0;
    }
}
