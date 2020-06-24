//
// Created by koalaa13 on 6/24/20.
//

#ifndef HW5_CORRECTOR_H
#define HW5_CORRECTOR_H

#include "Image.h"

struct Corrector {
    static void RGBCorrection(Image &image, double offset, double factor);

    static void YCbCrCorrection(Image &image, double offset, double factor);

    static void RGBAutoCorrection(Image &image);

    static void YCbCrAutoCorrection(Image &image);

    static void RGBSkipAutoCorrection(Image &image);

    static void YCbCrSkipAutoCorrection(Image &image);

private:
    static double doCorrection(double d, double maxValue, double offset, double factor);

    static double doAutoCorrection(double d, double maxValue, double mn, double mx);

    static std::tuple<double, double, double> toYCbCr601(double r, double g, double b);

    static std::tuple<double, double, double> fromYCbCr601(double y, double cb, double cr);
};


#endif //HW5_CORRECTOR_H
