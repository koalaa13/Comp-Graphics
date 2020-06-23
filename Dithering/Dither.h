//
// Created by koalaa13 on 6/22/20.
//

#ifndef HW3_DITHER_H
#define HW3_DITHER_H

#include "PNMImage.h"
#include "random"
#include "algorithm"
#include "cmath"
#include "string"
#include "vector"

struct Dither {
    static void randomDithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void noDithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void
    FloydSteinbergDithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void
    JarvisJudiceNinkeDithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void
    Sierra3Dithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void AtkinsonDithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void ordered8x8Dithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

    static void
    halftone4x4Dithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma);

private:
    static double *getData(PNMImage const &image, bool grad);

    static std::vector<double> getValues(int maxValue, int bit);

    static void drawGrad(double *data, PNMImage const &image) noexcept;

    static double getLeft(std::vector<double> const &values, double value);

    static double getRight(std::vector<double> const &values, double value);

    static double getValueWithGamma(double value, double maxValue, double gamma);

    static double sRGB(double value);
};


#endif //HW3_DITHER_H
