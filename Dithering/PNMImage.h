//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_PNMIMAGE_H
#define HW2_PNMIMAGE_H

#include <string>
#include <vector>
#include "ImageFile.h"
#include "cmath"
#include <iostream>
#include "algorithm"
#include <random>

typedef unsigned char byte;

struct PNMImage {

    explicit PNMImage(const ImageFile &imageFile, bool grad);

    ~PNMImage();

    void writeToFile(const ImageFile &imageFile);

    void ditheringFloydSteinberg(int bit, double gamma);

    void ditheringJarvisJudiceNinke(int bit, double gamma);

    void randomDithering(int bit, double gamma);

    void noDithering(int bit, double gamma);

    void ditheringSierra3(int bit, double gamma);

    void ditheringAtkinson(int bit, double gamma);

    void orderedDithering(int bit, double gamma);

    void halftone4x4Dithering(int bit, double gamma);

private:
    void fillHorizontalGrad();

    std::vector<byte> getValues(int bit) const;

    void setPixelColor(int x, int y, byte value);

    byte getValueWithGamma(byte value, double gamma) const;

    int width, height, pixelSize, IMAGE_SIZE;
    byte *data;
};


#endif //HW2_PNMIMAGE_H
