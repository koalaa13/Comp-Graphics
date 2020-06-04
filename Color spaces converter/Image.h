//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_PNMIMAGE_H
#define HW2_PNMIMAGE_H

#include <string>
#include <cstring>
#include <vector>
#include "ImageFile.h"
#include "cmath"
#include <iostream>
#include "algorithm"
#include "cstdio"

typedef unsigned char byte;

struct Image {

    Image(const Image &other);

    explicit Image(const ImageFile &imageFile);

    ~Image();

    void writeToFile(const ImageFile &imageFile) const;

    Image &operator=(Image const &other);

private:
    Image(const Image &image1, const Image &image2, const Image &image3);

    Image(const Image &other, int ind);

    int width, height, pixelSize, IMAGE_SIZE, BYTES_PER_PIXEL;
    byte *data;

    friend class Converter;
};


#endif //HW2_PNMIMAGE_H
