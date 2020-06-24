//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_PNMIMAGE_H
#define HW2_PNMIMAGE_H

#include <string>
#include "ImageFile.h"
#include "Line.h"
#include "cmath"
#include "algorithm"

typedef unsigned char byte;

struct PNMImage {

    explicit PNMImage(const ImageFile &imageFile);

    ~PNMImage();

    void writeToFile(const ImageFile &imageFile);

    void drawLine(int brightness, double thickness, int x0, int y0, int x1, int y1, double gamma, bool srgb);

private:
    byte getLineColor(Line const &line, int x, int y, double thickness) const;

    void setPixelColor(int x, int y, int intensity, int brightness, double gamma, bool srgb);

    static double sRGB(double u) ;

    static double revsRGB(double u) ;

    static double gammaCorrection(double u, double gamma, bool srgb, bool rev) ;

    int width, height, pixelSize, IMAGE_SIZE;
    byte *data;
};


#endif //HW2_PNMIMAGE_H
