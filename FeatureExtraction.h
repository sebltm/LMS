//
// Created by sebltm on 2/10/20.
//

#ifndef LMS_FEATUREEXTRACTION_H
#define LMS_FEATUREEXTRACTION_H


#define PI 3.1415926535

#include <IO.h>
#include <cmath>
#include <vector>
#include <fftw3.h>
#include <iostream>

class FeatureExtraction {
    typedef struct {
        float* real;
        float* comp;
    } FFT;
private:
    static void hamming(int, SAMPLE *);

public:
    static double ** STFT(std::vector<SAMPLE> *, int, int, int);
};


#endif //LMS_FEATUREEXTRACTION_H
