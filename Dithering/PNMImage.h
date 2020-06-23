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

    explicit PNMImage(const ImageFile &imageFile);

    ~PNMImage();

    void writeToFile(const ImageFile &imageFile) const;

private:
    friend class Dither;

    void writeHeaderToFile(const ImageFile &imageFile) const;

    int width, height, pixelSize, IMAGE_SIZE;
    byte *data;
};


#endif //HW2_PNMIMAGE_H
