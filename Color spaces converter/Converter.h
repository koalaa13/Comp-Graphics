//
// Created by koalaa13 on 6/1/20.
//

#ifndef HW4_CONVERTER_H
#define HW4_CONVERTER_H

#include "tuple"
#include "cmath"
#include "Image.h"
#include "ImageFile.h"
#include "string"

typedef unsigned char byte;

struct Converter {
    static Image convertRGBToHSL(const Image &input);

    static Image convertHSLToRGB(const Image &input);

    static Image merge(const Image &im1, const Image &im2, const Image &im3);

    static std::tuple<Image, Image, Image> split(const Image &im);

    static std::tuple<double, double, double> fromHSL(double h, double s, double l);

    static std::tuple<double, double, double> toHSL(double r, double g, double b);
};

#endif //HW4_CONVERTER_H
