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

    explicit Image(const ImageFile &imageFile);

    ~Image();

    void writeToFile(const ImageFile &imageFile) const;

    Image &operator=(Image const &other);

    friend bool operator==(Image const &a, Image const &b);

private:
    friend class Corrector;

    int width, height, pixelSize, IMAGE_SIZE, BYTES_PER_PIXEL;
    byte *data;
};


#endif //HW2_PNMIMAGE_H
