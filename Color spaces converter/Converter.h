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

    static Image convertRGBToHSV(const Image &input);

    static Image convertHSVToRGB(const Image &input);

    static Image convertRGBToYCbCr601(const Image &input);

    static Image convertYCbCr601ToRGB(const Image &input);

    static Image convertRGBToYCbCr709(const Image &input);

    static Image convertYCbCr709ToRGB(const Image &input);

    static Image convertRGBToYCoCg(const Image &input);

    static Image convertYCoCgToRGB(const Image &input);

    static Image convertRGBToCMY(const Image &input);

    static Image convertCMYToRGB(const Image &input);

    static Image merge(const Image &im1, const Image &im2, const Image &im3);

    static std::tuple<Image, Image, Image> split(const Image &im);

private:
    static std::tuple<double, double, double> fromHSL(double h, double s, double l);

    static std::tuple<double, double, double> toHSL(double r, double g, double b);

    static std::tuple<double, double, double> fromHSV(double h, double s, double v);

    static std::tuple<double, double, double> toHSV(double r, double g, double b);

    static std::tuple<double, double, double> fromYCbCr601(double y, double cb, double cr);

    static std::tuple<double, double, double> toYCbCr601(double r, double g, double b);

    static std::tuple<double, double, double> fromYCbCr709(double y, double cb, double cr);

    static std::tuple<double, double, double> toYCbCr709(double r, double g, double b);

    static std::tuple<double, double, double> fromYCoCg(double y, double co, double cg);

    static std::tuple<double, double, double> toYCoCg(double r, double g, double b);

    static std::tuple<double, double, double> fromCMY(double c, double m, double y);

    static std::tuple<double, double, double> toCMY(double r, double g, double b);
};

#endif //HW4_CONVERTER_H
