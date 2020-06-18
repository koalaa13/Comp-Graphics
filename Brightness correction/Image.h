//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_PNMIMAGE_H
#define HW2_PNMIMAGE_H

#include <string>
#include <cstring>
#include "ImageFile.h"
#include "cmath"
#include "algorithm"
#include "cstdio"
#include "tuple"
#include "vector"

typedef unsigned char byte;

struct Image {

    Image(const Image &other);

    explicit Image(const ImageFile &imageFile, bool colorSpace);

    ~Image();

    void writeToFile(const ImageFile &imageFile) const;

    Image &operator=(Image const &other);

    void correction(double offset, double factor);

    void autoCorrection();

    void autoSkipCorrection();

private:
    static double getBrightness(double const &r, double const &g, double const &b);

    double doAutoCorrection(double const &d, double const &mn, double const &mx) const;

    double doCorrection(double const &d, double const &offset, double const &factor) const;

    int width, height, pixelSize, IMAGE_SIZE, BYTES_PER_PIXEL;
    bool colorSpace;  // 0 - RGB, 1 - YCbCr
    byte *data;
};


#endif //HW2_PNMIMAGE_H
